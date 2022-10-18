#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <utility>
#include "boost/asio/read_until.hpp"
#include "LicensePlateListener.h"


using shared_socket = std::shared_ptr<boost::asio::ip::tcp::socket>;

class SocketLicensePlateListener :
        public LicensePlateListener {
    boost::asio::io_service service_;
    std::shared_ptr<std::thread> workingThread_;
    std::atomic<bool> inWork {false};

    std::string ip;
    int port;

    std::string read(shared_socket);
    void connect();
    void start_reading(shared_socket& socket, const boost::system::error_code& error);

public:
    SocketLicensePlateListener(std::string  ip,int port ) :
    ip(std::move(ip)),
    port(port)
    {}

    void stop() override;
    void run() override;
};

