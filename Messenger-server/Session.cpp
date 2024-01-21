#include "Session.h"


Session::Session(boost::asio::io_context& ioContext, tcp::socket socket)
    : ioContext_(ioContext), socket_(std::move(socket)), timer_(ioContext) {
    std::cout << 5 << std::endl;
}

void Session::start() {
    std::cout << 6 << std::endl;
    doRead();
}

void Session::doRead() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_),
        [this, self](boost::system::error_code ec, std::size_t length) {
            std::cout << 7 << std::endl;
            if (!ec) {
                std::cout << 8 << std::endl;
                doWrite(length);
            }
            else {
                std::cout << 7.1 << std::endl;
            }
        }
    );

    timer_.expires_after(std::chrono::seconds(1));
    timer_.async_wait([this, self](boost::system::error_code ec) {
        if (ec == boost::asio::error::operation_aborted) {
        }
        else {
            handleTimeout();
        }
        });
}

void Session::doWrite(std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t) {
            std::cout << 9 << std::endl;
            if (!ec) {
                std::cout << 10 << std::endl;
                doRead();
            }
            else {
                // Обработка ошибок записи
            }
        }
    );
}

void Session::handleTimeout() {
    std::cout << 11 << std::endl;
    boost::asio::post(ioContext_, [this]() {
        doRead();
        });
}