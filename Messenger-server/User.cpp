#include "User.h"
#include <stdexcept>

User::User(std::string username, std::string first_name, std::string second_name) {
	this->username = username;
	this->first_name = first_name;
	this->second_name = second_name;
	this->histories_messaging = std::map<std::string, std::shared_ptr<History_messaging>>();
	mtx_histories_messaging = new std::mutex();
}

std::string User::get_username() {
	return username;
}

std::string User::get_first_name() {
	return first_name;
}

std::string User::get_second_name() {
	return second_name;
}

std::shared_ptr<History_messaging> User::get_history_messaging(User& user) {
	std::lock_guard<std::mutex> guard(*mtx_histories_messaging);
	if (histories_messaging.count(user.get_username()) == 0) {
		auto ptr = std::make_shared<History_messaging>(History_messaging(this->username, user.get_username()));
		
		histories_messaging[user.get_username()] = user.set_history_messaging(ptr);
	}

	return histories_messaging[user.get_username()];
}

std::shared_ptr<History_messaging> User::set_history_messaging(std::shared_ptr<History_messaging> history) {
	if ((*history).get_username_1() != this->username && (*history).get_username_2() != this->username) {
		throw std::logic_error("The message history must contain the current user!");
	}


	std::string username = (*history).get_username_1();
	if (username == this->username) {
		username = (*history).get_username_2();
	}

	std::lock_guard<std::mutex> guard(*mtx_histories_messaging);
	if (histories_messaging.count(username) == 0)
		this->histories_messaging[username] = history;

	return this->histories_messaging[username];
}

bool User::operator == (const User& user) const {
	return this->username == user.username;
}

bool User::operator != (const User& user) const {
	return this->username != user.username;
}

User::~User()
{
	delete mtx_histories_messaging;
}