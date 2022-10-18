//
// Created by lev on 08.10.22.
//

#include "SocketLicensePlateListener.h"

using namespace boost::asio;
using boost::system::error_code;
using namespace boost::placeholders;

void SocketLicensePlateListener::run() {
    inWork = true;

    workingThread_ = std::make_shared<std::thread>(std::thread(
            [&](){

                while(inWork){
                    connect();

                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    std::cout<<"Reconnection...\n";
                }
            })
                    );
}

void SocketLicensePlateListener::connect() {
    ip::tcp::endpoint ep(ip::tcp::endpoint(ip::address::from_string(ip), port));
    auto socket_ = std::make_shared<ip::tcp::socket>(ip::tcp::socket(service_));
    error_code error;
    std::cout<<"Wait connection...\n";

    try {
        socket_->connect(ep, error);
        start_reading(socket_, error);
    } catch(boost::wrapexcept<boost::system::system_error>& ex) {
        std::cout<<ex.what()<<'\n';
    }

    socket_->close();
    std::cout<<"CLose socket\n";
}

void SocketLicensePlateListener::start_reading(shared_socket &socket,
                                               const error_code &error) {
    if (error) {
        std::cout << "Error accepting connection: " << error.message()
                  << std::endl;
        return;
    }

    while (inWork) {
        auto msg = read(socket);
        if (msg.empty()) {
            break;
        }
        pushPlate(msg);
        std::cout << "RECEIVED: " << msg << std::endl;
    }
}

std::string SocketLicensePlateListener::read(shared_socket socket_) {

    char buf[1024];
    size_t bytes = 0;
    try
    {
        bytes = socket_->receive(buffer(buf));

    } catch(boost::wrapexcept<boost::system::system_error>& ex){
        std::cout<<ex.what()<<"\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        return "";
    } catch(boost::exception &ex){
        std::cout<<"Unknown READ exception : "<<diagnostic_information_what(ex)<<"\n";
        return "";
    }

    return  std::string(buf, bytes - 1);
}

void SocketLicensePlateListener::stop(){
    inWork = false;
    workingThread_->join();
    std::cout<<"Stop client\n";
}