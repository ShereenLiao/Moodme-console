#include <iostream>
#include "../include/client.h"

int main() {
    try{
        boost::asio::io_service io_service;
        client * app = new client(io_service);
        app->run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}
