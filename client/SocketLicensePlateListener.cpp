//
// Created by lev on 08.10.22.
//

#include "SocketLicensePlateListener.h"

size_t SocketLicensePlateListener::read_complete(char *buf, const boost::system::error_code &err, size_t bytes) {
    if ( err) return 0;
    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void SocketLicensePlateListener::sync_echo(std::string msg) {
    msg += "\n";
    ip::tcp::socket sock(service);
    sock.connect(ep);

    std::cout<<"Connected...\n";

    std::cout<<"Write to server\n";
    sock.write_some(buffer(msg));

    std::cout<<"Start reading...\n";
    char buf[1024];
    int bytes = read(sock, buffer(buf), boost::bind(read_complete,buf,_1,_2));
    std::string copy(buf, bytes - 1);

    std::cout << "server echoed our " << msg << " : "
              << copy << std::endl;
    std::cout<<"Close socket...\n";

    sock.close();
}

void SocketLicensePlateListener::start(){
    int a = 0;
}
void SocketLicensePlateListener::stop(){
    int a = 0;
}