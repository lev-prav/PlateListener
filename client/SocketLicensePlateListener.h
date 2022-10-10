#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include "LicensePlateListener.h"

using namespace boost::asio;
using boost::system::error_code;
using namespace boost::placeholders;


class SocketLicensePlateListener :
        public LicensePlateListener {
    io_service service_;
    ip::tcp::endpoint ep_;
    ip::tcp::socket sock_;
public:

    SocketLicensePlateListener():
    sock_(service_),
    ep_( ip::address::from_string("127.0.0.1"), 8001){}

    static size_t read_complete(char * buf, const error_code & err, size_t bytes);
    void sync_echo(std::string msg);
    virtual void start() override;
    virtual void stop() override;

    void read();

};

