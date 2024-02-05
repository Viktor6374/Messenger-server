#pragma once
#include <vector>
#include "Message.h"

class Message;

class History_messaging : public std::enable_shared_from_this<History_messaging>
{
private:
	std::string username_1;
	std::string username_2;
	std::vector<Message> messages;
	std::mutex * mtx_messages;
public:
	History_messaging(std::string username_1, std::string username_2);
	History_messaging(const History_messaging& other);
	void add_message(Message message);
	std::string get_username_1();
	std::string get_username_2();
	std::vector<Message> get_messages();
	~History_messaging();
};

