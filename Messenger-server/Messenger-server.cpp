#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "ServerListener.h"

int main() {
    try {
        std::cout << 0 << std::endl;
        boost::asio::io_context ioContext;
        ServerListener server(ioContext, 9000, 4);
        ioContext.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}