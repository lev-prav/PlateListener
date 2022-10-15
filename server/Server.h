#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <queue>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;
using namespace boost::placeholders;

typedef std::shared_ptr<ip::tcp::socket> shared_socket;
typedef std::shared_ptr<ip::tcp::acceptor> shared_acceptor;

class PlateServer{
public:

    void run(){
        inWork = true;

        dataThread = std::make_unique<std::thread>(
                std::thread(
                        [&]() {
                            char i = 47;
                            char plate[20] = "AU1488EB ";
                            plate[10] = '\0';
                            while (inWork) {

                                {
                                    std::lock_guard<std::mutex> locker(mut_);
                                    plate[8] = i++;

                                    plates_.push(std::string(plate));
                                }
                                std::cout<<plate;
                                boost::this_thread::sleep(boost::posix_time::millisec(10000));
                            }
                        }));


        sockThread = std::make_unique<std::thread>(
                [&](){
                    shared_acceptor acceptor_ =  std::make_shared<ip::tcp::acceptor>(
                            ip::tcp::acceptor(service_, ip::tcp::endpoint(ip::tcp::v4(),8081))
                    );

                    start(acceptor_);

                    service_.run();
                    std::cout<<"Service stopped\n";
                }
        );

    }

    void stop(){
        inWork = false;
        service_.stop();

        dataThread->join();
        sockThread->join();
    }

    void handle_connections() {
        ip::tcp::acceptor acceptor(service_, ip::tcp::endpoint(ip::tcp::v4(),8001));
        char buff[1024];
        while ( true) {
            ip::tcp::socket sock(service_);
            std::cout<<"Socket created, start accepting...\n";
            acceptor.accept(sock);

            std::cout<<"Start listening ...\n";

            int bytes = read(sock, buffer(buff),
                             boost::bind(read_complete,buff,_1,_2));
            std::string msg(buff, bytes);

            std::cout<<"RECIEVED :"<< msg <<std::endl;
            boost::this_thread::sleep( boost::posix_time::millisec(5000));

            sock.write_some(buffer(msg));
            sock.close();
        }
    }

private:

    io_service service_;
    std::mutex mut_;
    std::queue<std::string> plates_;
    std::atomic<bool> inWork { false };

    std::unique_ptr<std::thread> dataThread;
    std::unique_ptr<std::thread> sockThread;

    void start(const shared_acceptor& acceptor_){

        shared_socket socket_ = std::make_shared<ip::tcp::socket>(ip::tcp::socket(acceptor_->get_executor()));;

        std::cout<<"Socket created, start accepting...\n";
        acceptor_->async_accept(*socket_, boost::bind(&PlateServer::onAccept,
                                                      this,
                                                      socket_,
                                                      acceptor_,
                                                      boost::asio::placeholders::error)
        );
    }

    void onAccept(shared_socket& socket,
                  const shared_acceptor& acceptor,
                  const boost::system::error_code& error){
        std::cout<<"Socket accepted \n";

        if ( error )
        {
            std::cout << "Error accepting connection: " << error.message()
                      << std::endl;
            return;
        }

        while(inWork){
            boost::this_thread::sleep( boost::posix_time::millisec(1000));
            sendPlates(socket);
        }

    }

    void sendPlates(shared_socket & socket_){
        //TODO не будет ли проблем с эмпти без мьютекса
        while(!plates_.empty()){
            std::string plate;
            {
                std::lock_guard<std::mutex> locker(mut_);
                plate = plates_.front();
                plates_.pop();
            }
            std::cout<<"Send plates\n";
            socket_->write_some(buffer(plate));
        }
    }

    void closeSocket(shared_socket& socket_){
        std::cout<<"Close sockets\n";
        socket_->close();

    }


    static size_t read_complete(char * buff, const error_code & err, size_t bytes) {
        if ( err) return 0;
        bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }
};