#include <iostream>
#include "Server.h"


int main(int argc, char* argv[]) {

    PlateServer server;
    std::cout<<"Start server on 127.0.0.1 8001\n";

    //server.handle_connections();
    server.run();
    std::cout<<"Press ANY KEY to STOP\n";

    getchar();

    server.stop();
    std::cout<<"Stop server\n";
}