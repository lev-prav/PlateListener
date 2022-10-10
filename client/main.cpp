#include "SocketLicensePlateListener.h"


int main() {
    std::cout<<"Connect to 127.0.0.1\n";

    std::string msg("Hello, World!");
    SocketLicensePlateListener socket;
    boost::thread thread(
            [msg, &socket] {
                socket.start();
                while(true)
                    socket.read();
                socket.stop();
                return socket.sync_echo(msg);
            });
    thread.join();

    std::cout<<"Finish\n";
    return 0;
}
