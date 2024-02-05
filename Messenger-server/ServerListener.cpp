#include "ServerListener.h"

ServerListener::ServerListener(boost::asio::io_context& ioContext, short port, std::size_t numThreads)
    : acceptor_(ioContext, tcp::endpoint(tcp::v6(), port)) {
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
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::shared_ptr<Session> session = std::make_shared<Session>(ioContext_, std::move(socket));
                session.get()->initialize_context();
                session.get()->doRead();
            }
            startAccept();
        }
    );
}