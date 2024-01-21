#include "ServerListener.h"

ServerListener::ServerListener(boost::asio::io_context& ioContext, short port, std::size_t numThreads)
    : acceptor_(ioContext, tcp::endpoint(tcp::v6(), port)) {
    std::cout << 1 << std::endl;
    startAccept();
    for (std::size_t i = 0; i < numThreads; ++i) {
        threadPool_.create_thread([this]() {
            ioContext_.run();
            });
    }
}

ServerListener::~ServerListener() {
    threadPool_.join_all();
}

void ServerListener::startAccept() {
    std::cout << 2 << std::endl;
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            std::cout << 3 << std::endl;
            if (!ec) {
                std::cout << 4 << std::endl;
                std::make_shared<Session>(ioContext_, std::move(socket))->start();
            }
            startAccept();
        }
    );
}