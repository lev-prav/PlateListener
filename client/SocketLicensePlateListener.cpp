//
// Created by lev on 08.10.22.
//

#include "SocketLicensePlateListener.h"


void SocketLicensePlateListener::run() {
    inWork = true;

    workingThread_ = std::make_shared<std::thread>(std::thread(
            [&](){

                while(inWork){
                    auto socketThread = makeSocketThread();

                    socketThread->join();
                    boost::this_thread::sleep(boost::posix_time::millisec(1000));
                    std::cout<<"Reconnection...\n";
                }
            })
                    );

}

std::shared_ptr<std::thread> SocketLicensePlateListener::makeSocketThread() {
    return std::make_shared<std::thread>(
            [&](){

                ip::tcp::endpoint ep(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 8081));
                auto socket_ = std::make_shared<ip::tcp::socket>(ip::tcp::socket(service_));
                error_code error;
                std::cout<<"Wait connection...\n";

                try {
                    socket_->connect(ep, error);
                    startReading(socket_, error);
                } catch(boost::wrapexcept<boost::system::system_error>& ex) {
                    std::cout<<ex.what()<<'\n';
                }

                socket_->close();
                std::cout<<"CLose socket\n";
            }
    );
}

void SocketLicensePlateListener::startReading(shared_socket &socket,
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
        boost::this_thread::sleep(boost::posix_time::millisec(500));

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