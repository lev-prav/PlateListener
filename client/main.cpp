#include "SocketLicensePlateListener.h"


int main() {
    std::cout<<"Connect to 127.0.0.1\n";

    std::string msg("Hello, World!");
    SocketLicensePlateListener listener;

    listener.run();

    std::cout<<"Finish\n";
    return 0;
}
