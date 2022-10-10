#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;
using namespace boost::placeholders;

class PlateServer{
    io_service service;

public:
    static size_t read_complete(char * buff, const error_code & err, size_t bytes) {
        if ( err) return 0;
        bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }
    void handle_connections() {
        ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8001));
        char buff[1024];
        while ( true) {
            ip::tcp::socket sock(service);
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