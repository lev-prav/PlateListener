//
// Created by lev on 08.10.22.
//

#include "SocketLicensePlateListener.h"


void SocketLicensePlateListener::stop(){
    socket_->close();
    service_.stop();
    std::cout<<"CLose socket\n";
}

void SocketLicensePlateListener::run() {
    thread_ = std::make_unique<std::thread>(
            [&](){
//                start();
//                readAll();
//                stop();

                ip::tcp::endpoint ep(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 8081));
                socket_ = std::make_shared<ip::tcp::socket>(ip::tcp::socket(service_));

                std::cout<<"Wait connection...\n";

                socket_->async_connect(ep, boost::bind(
                        &SocketLicensePlateListener::onConnect,
                        this,
                        socket_,
                        boost::asio::placeholders::error
                        )
                );

                service_.run();
            }
            );
    thread_->join();
}

void SocketLicensePlateListener::onConnect(shared_socket &socket,
                                           const error_code &error) {
    if (error){
        std::cout << "Error accepting connection: " << error.message()
                  << std::endl;
        return;
    }
    readAll();
    stop();
}

void SocketLicensePlateListener::readAll() {
    std::cout<<"Start reading...\n";
    std::string msg = read();
    while(!msg.empty()) {
        pushPlate(msg);
        std::cout << "RECEIVED: " << msg << std::endl;
        msg = read();
    }
}

std::string SocketLicensePlateListener::read() {
    char buf[1024];
    size_t red_buf = 0;
    boost::system::error_code err;
    unsigned long  bytes;
    try{
        bytes = boost::asio::read(*socket_, buffer(buf),
                                       boost::bind(&SocketLicensePlateListener::read_complete,
                                                   this,
                                                   buf,
                                                   err,
                                                   red_buf)
        );
    } catch(boost::exception& ex){
        std::cout<< diagnostic_information_what(ex)<<"\n";
        return "";
    }

    return  std::string(buf, bytes - 1);
}

/* RETURN a number of byt*s to read*/
//TODO добавить размер считывания, можно хардкодом
size_t SocketLicensePlateListener::read_complete(char *buf, const boost::system::error_code &err, size_t& bytes) {
    if (err || err == boost::asio::error::eof)
        return 0;

    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    bytes++;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void SocketLicensePlateListener::sync_echo(std::string msg) {
    msg += "\n";
    ip::tcp::socket sock(service_);
    //sock.connect(*ep_);

    std::cout<<"Connected...\n";

    std::cout<<"Write to server\n";
    sock.write_some(buffer(msg));

    std::cout<<"Start reading...\n";
    char buf[1024];
    int bytes = 0; //boost::asio::read(sock, buffer(buf), boost::bind(read_complete,buf,_1,_2));
    std::string copy(buf, bytes - 1);

    std::cout << "server echoed our " << msg << " : "
              << copy << std::endl;
    std::cout<<"Close socket...\n";

    sock.close();
}

void SocketLicensePlateListener::start() {
    std::cout<<"START\n";
}
