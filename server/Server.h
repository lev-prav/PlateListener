#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <queue>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;
using namespace boost::placeholders;

class PlateServer{
    std::vector<std::shared_ptr<ip::tcp::socket>> acceptedSockets_;
    std::unique_ptr<ip::tcp::acceptor> acceptor_;
    io_service service_;
    std::mutex mut_;
    std::queue<std::string> plates_;
    std::atomic<bool> inWork { true };

    std::unique_ptr<std::thread> dataThread;
    std::unique_ptr<std::thread> sockThread;
public:

    PlateServer(){
        acceptor_ = std::make_unique<ip::tcp::acceptor>(
                ip::tcp::acceptor(service_, ip::tcp::endpoint(ip::tcp::v4(),8001))
        );
    }

    static size_t read_complete(char * buff, const error_code & err, size_t bytes) {
        if ( err) return 0;
        bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }

    void acceptSocket(){
        auto sock = std::make_shared<ip::tcp::socket>(ip::tcp::socket(service_));
        std::cout<<"Socket created, start accepting...\n";
        acceptor_->accept(*sock);
        acceptedSockets_.push_back(sock);
    }

    void sendPlate(const std::string& plate){
        std::cout<<"Send plates\n";
        for (const auto& sock : acceptedSockets_){
            sock->write_some(buffer(plate));
        }
    }

    void broadcastQueue(){
        while(!plates_.empty()){
            auto& plate = plates_.front();
            sendPlate(plate);
            plates_.pop();
        }
    }

    void closeSockets(){
        std::cout<<"Close sockets\n";
        for (const auto& sock : acceptedSockets_){
            sock->close();
        }
    }

    void run(){

        dataThread = std::make_unique<std::thread>(
                std::thread(
                        [&](){

                            while(inWork){
                                boost::this_thread::sleep(boost::posix_time::millisec(3000));
                                std::lock_guard<std::mutex> locker(mut_);
                                plates_.push("AU1488EB\n");
                            }
                        }));

        acceptSocket();

        if (!plates_.empty()){

        }
    }

    void sendStop(){
        sendPlate("\n");
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

};