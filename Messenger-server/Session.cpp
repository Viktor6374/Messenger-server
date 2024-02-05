#include "Session.h"


Session::Session(boost::asio::io_context& ioContext, tcp::socket socket)
    : ioContext_(ioContext), socket_(std::move(socket)), timer_(ioContext) {
    context = std::make_shared<SessionContext>();
    mtx = new std::mutex;
}

void Session::doRead() {
    std::cout << "Waiting for a message" << std::endl;
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_),
        [this, self](boost::system::error_code ec, std::size_t length) {
            std::cout << "The message has been received " << std::endl;
            if (!ec) {
                handleRead(length);
                doRead();
            }
            else {
                std::cout << ec.message() << std::endl;
                if (ec = boost::asio::error::eof) {
                    return;
                }
                doRead();
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

void Session::initialize_context() {
    std::shared_ptr<Session> s = shared_from_this();
    context->set_session(s);
}

void Session::doWrite(std::string response) {
    std::lock_guard<std::mutex> guard(*mtx);
    std::array<char, 10000> buff;

    response += "/n";

    for (int i = 0; i < response.length(); i++) {
        buff[i] = response[i];
    }

    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(buff, response.length()),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                std::cout << "The answer is recorded:" << std::endl;
            }
            else {
                std::cout << ec.message() << std::endl;
            }
        }
    );
    std::cout << response << std::endl;
}

void Session::handleTimeout() {
    std::cout << "The waiting time has expired" << std::endl;
    boost::asio::post(ioContext_, [this]() {
        doRead();
        });
}

void Session::handleRead(std::size_t length) {
    std::string str(data_.begin(), data_.begin() + length);
    std::cout << str << std::endl;
    std::string buff = remains + str;

    size_t pos_delimiter = buff.find(delimiter);

    if (pos_delimiter == std::string::npos) {
        remains = buff;
        return;
    }

    std::string mess = buff.substr(0, pos_delimiter);

    context->set_message(mess);

    remains = buff.substr(pos_delimiter + 2);
}