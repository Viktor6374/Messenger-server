#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include "SessionContext.h"

using boost::asio::ip::tcp;

class SessionContext;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::io_context& ioContext, tcp::socket socket);
    void doRead();

    void doWrite(std::string response);
    void initialize_context();
private:
    std::mutex* mtx;
    void handleTimeout();
    void handleRead(std::size_t length);

    boost::asio::io_context& ioContext_;
    tcp::socket socket_;
    std::array<char, 10000> data_;
    boost::asio::steady_timer timer_;
    const std::string delimiter = "/n";
    std::shared_ptr<SessionContext> context;
    std::string remains;
};

