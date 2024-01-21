#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include "./Session.h"

using boost::asio::ip::tcp;

class ServerListener {
public:
    ServerListener(boost::asio::io_context& ioContext, short port, std::size_t numThreads);

    ~ServerListener();

private:
    void startAccept();

    tcp::acceptor acceptor_;
    boost::asio::io_context ioContext_;
    boost::thread_group threadPool_;
};

