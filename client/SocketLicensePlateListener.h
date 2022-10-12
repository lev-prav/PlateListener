#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include "boost/asio/read_until.hpp"
#include "LicensePlateListener.h"

using namespace boost::asio;
using boost::system::error_code;
using namespace boost::placeholders;

typedef std::shared_ptr<ip::tcp::socket> shared_socket;

class SocketLicensePlateListener :
        public LicensePlateListener {
    io_service service_;
    shared_socket socket_;
    std::unique_ptr<std::thread> thread_;
    std::atomic<bool> inWork {false};

    std::string read();
    void readAll();
    void onRead(char* buf,
                const error_code &error,
                size_t& bytes);
    void onConnect(shared_socket& socket, const boost::system::error_code& error);
    size_t read_complete(char * buf, const error_code & err, size_t& bytes);

public:


    void sync_echo(std::string msg);
    void start() override;
    void stop() override;
    void run() override;

};

