#pragma once
#include "User.h"
#include <string>
#include <ctime>


class Message
{
private:
	std::string sender_username;
	std::string addressee_username;
	std::string message;
	std::time_t send_time;
public:
	Message(std::string sender_username, std::string addressee_username, std::string message, std::time_t send_time);

	std::string get_sender_username();

	std::string get_addresses_username();

	std::string get_message();

	std::time_t get_send_time();
};

