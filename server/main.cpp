#include <iostream>
#include "Server.h"


int main(int argc, char* argv[]) {
    std::cout<<"Start server on 127.0.0.1 8001\n";

    PlateServer server;

    server.handle_connections();

    std::cout<<"Stop server\n";
}