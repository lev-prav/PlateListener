#include "SocketLicensePlateListener.h"


int main() {
    std::cout<<"Connect to 127.0.0.1\n";

    SocketLicensePlateListener listener;

    std::cout<<"Press ANY key to stop\n";

    listener.run();

    getchar();

    listener.stop();

    std::cout<<"Finish\n";
    return 0;
}
