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
    std::shared_ptr<std::thread> workingThread_;
    std::atomic<bool> inWork {false};

    std::string read(shared_socket);
    std::shared_ptr<std::thread> make_socket_thread();
    void start_reading(shared_socket& socket, const boost::system::error_code& error);

public:

    void stop() override;
    void run() override;
};

