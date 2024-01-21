#pragma once
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::io_context& ioContext, tcp::socket socket);

    void start();

private:
    void doRead();

    void doWrite(std::size_t length);

    void handleTimeout();

    boost::asio::io_context& ioContext_;
    tcp::socket socket_;
    std::array<char, 1024> data_;
    boost::asio::steady_timer timer_;
};

