#include "Message.h"


Message::Message(std::string sender_username, std::string addressee_username, std::string message, std::time_t send_time){
	this->sender_username = sender_username;
	this->addressee_username = addressee_username;
	this->message = message;
	this->send_time = send_time;
}

std::string Message::get_sender_username() {
	return sender_username;
}

std::string Message::get_addresses_username() {
	return addressee_username;
}

std::string Message::get_message() {
	return message;
}

std::time_t Message::get_send_time() {
	return send_time;
}