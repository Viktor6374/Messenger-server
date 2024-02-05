#include "User.h"
#include <stdexcept>

User::User(std::string username, std::string first_name, std::string second_name) {
	this->username = username;
	this->first_name = first_name;
	this->second_name = second_name;
	this->histories_messaging = std::vector<std::shared_ptr<History_messaging>>();
	mtx_histories_messaging = new std::mutex();
}

User::User(const User& other) {
	this->username = std::string(other.username);
	this->first_name = std::string(other.first_name);
	this->second_name = std::string(other.second_name);
	this->histories_messaging = other.histories_messaging;
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

std::shared_ptr<History_messaging> User::get_history_messaging(std::string username) {
	std::lock_guard<std::mutex> guard(*mtx_histories_messaging);
	for (int i = 0; i < histories_messaging.size(); i++) {
		if (histories_messaging[i].get()->get_username_1() == username || histories_messaging[i].get()->get_username_2() == username) {
			return histories_messaging[i];
		}
	}

	return nullptr;
}

std::shared_ptr<History_messaging> User::get_history_messaging(int index) {
	return histories_messaging[index];
}

std::shared_ptr<History_messaging> User::set_history_messaging(std::shared_ptr<History_messaging> history) {
	if ((*history).get_username_1() != this->username && (*history).get_username_2() != this->username) {
		throw std::logic_error("The message history must contain the current user!");
	}

	std::string interlocutor_username(history.get()->get_username_1());
	if (interlocutor_username == username) {
		interlocutor_username = history.get()->get_username_2();
	}

	std::lock_guard<std::mutex> guard(*mtx_histories_messaging);
	for (int i = 0; i < histories_messaging.size(); i++) {
		if ((histories_messaging[i].get()->get_username_1() == interlocutor_username) || (histories_messaging[i].get()->get_username_2() == interlocutor_username)) {
			histories_messaging.erase(histories_messaging.begin() + i);
		}
	}

	histories_messaging.push_back(history);

	return history;
}

bool User::operator == (const User& user) const {
	return this->username == user.username;
}

bool User::operator != (const User& user) const {
	return this->username != user.username;
}

std::size_t User::get_size_histories_messaging() {
	return histories_messaging.size();
}

bool User::check_having_history_messaging(std::string username_) {
	std::lock_guard<std::mutex> guard(*mtx_histories_messaging);
	for (int i = 0; i < histories_messaging.size(); i++) {
		if ((histories_messaging[i].get()->get_username_1() == username_) || (histories_messaging[i].get()->get_username_2() == username_)) {
			return true;
		}
	}

	return false;
}

User::~User()
{
	delete mtx_histories_messaging;
}