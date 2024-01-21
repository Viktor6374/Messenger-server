#include "History_messaging.h"
#include <stdexcept>

History_messaging::History_messaging(std::string username_1, std::string username_2) {
	if (username_1 == username_2) {
		throw std::logic_error("The user cannot have a chat with himself!");
	}
	this->username_1 = username_1;
	this->username_2 = username_2;
	messages = std::vector<Message>();
	mtx_messages = new std::mutex();
}

void History_messaging::add_message(Message message) {
	if (message.get_addresses_username() != username_1 && message.get_addresses_username() != username_2) {
		throw std::logic_error("The addressee is not among the participants of the dialogue!");
	}

	if (message.get_sender_username() != username_1 && message.get_sender_username() != username_2) {
		throw std::logic_error("The sender is not among the participants of the dialogue!");
	}

	std::lock_guard<std::mutex> guard(*mtx_messages);
	messages.push_back(message);
}

std::string History_messaging::get_username_1() {
	return username_1;
}

std::string History_messaging::get_username_2() {
	return username_2;
}

std::vector<Message> History_messaging::get_messages() {
	std::lock_guard<std::mutex> guard(*mtx_messages);
	return std::vector<Message>(messages);
}


History_messaging::~History_messaging()
{
	delete mtx_messages;
}