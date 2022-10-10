#include <iostream>
#include "Server.h"


int main(int argc, char* argv[]) {

    PlateServer server;
    std::cout<<"Start server on 127.0.0.1 8001\n";

    //server.handle_connections();
    server.run();
    server.sendPlate("AU1488EB\n");
    std::cout<<"SEND\n";
    server.sendPlate("AU1488EB\n");
    std::cout<<"SEND\n";
    server.sendPlate("AU1488EB\n");
    std::cout<<"SEND\n";
    server.sendStop();

    std::cout<<"Stop server\n";
}