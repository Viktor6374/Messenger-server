#include "InMemory.h"

std::shared_ptr<InMemory> InMemory::instance = nullptr;
boost::mutex * InMemory::static_mtx = new boost::mutex;

std::shared_ptr<InMemory> InMemory::get_instance() {
	if (instance == nullptr) {
		static_mtx->lock();
		if (instance == nullptr) {
			InMemory* mem = new InMemory();
			std::shared_ptr<InMemory> instance_(mem);
			instance = instance_;
			instance.get()->init();
		}
		static_mtx->unlock();
	}
	return instance;
}

void InMemory::init() {
	User user1("user", "name", "surname");
	User user2("user2", "name2", "surname2");
	User user3("user3", "name3", "surname3");
	users.push_back(user1);
	users.push_back(user2);
	users.push_back(user3);
	logins_and_passwords["user"] = "user";
	logins_and_passwords["user2"] = "user2";
	logins_and_passwords["user3"] = "user3";
}

InMemory::InMemory() {
	mtx = new std::recursive_mutex;
}

void InMemory::save_new_user(User new_user, std::string password) {
	std::lock_guard<std::recursive_mutex> guard(*mtx);
	for (int i = 0; i < users.size(); i++) {
		if (new_user == users[i]) {
			throw std::logic_error("The user already exists");
		}
	}

	User user(new_user.get_username(), new_user.get_first_name(), new_user.get_second_name());
	logins_and_passwords[new_user.get_username()] = password;
	users.push_back(user);
}

void InMemory::save_message(Message message){
	std::lock_guard<std::recursive_mutex> guard(*mtx);
	for (int i = 0; i < users.size(); i++) {
		if (message.get_sender_username() == users[i].get_username()) {
			users[i].get_history_messaging(message.get_addresses_username()).get()->add_message(message);
			return;
		}
	}

	throw std::logic_error("User not found");
}

void InMemory::save_history_messaging(History_messaging history) {
	std::shared_ptr<History_messaging> hist = std::make_shared<History_messaging>(history);

	std::lock_guard<std::recursive_mutex> guard(*mtx);
	for (int i = 0; i < users.size(); i++) {
		if (users[i].get_username() == history.get_username_1() || users[i].get_username() == history.get_username_2()) {
			users[i].set_history_messaging(hist);
		}
	}
}

std::shared_ptr<User> InMemory::initialize_user(std::string username, std::string password) {
	std::lock_guard<std::recursive_mutex> guard(*mtx);
	if (logins_and_passwords[username] != password) {
		throw std::logic_error("password not correct");
	}

	return get_user(username);
}

std::shared_ptr<User> InMemory::get_user(std::string username) {
	std::lock_guard<std::recursive_mutex> guard(*mtx);
	for (int i = 0; i < users.size(); i++) {
		if (users[i].get_username() == username) {
			return create_user(i);
		}
	}

	throw std::logic_error("User not found");
}

std::shared_ptr<User> InMemory::create_user(int index) {
	std::lock_guard<std::recursive_mutex> guard(*mtx);
	std::shared_ptr<User> user = std::make_shared<User>(users[index].get_username(), users[index].get_first_name(), users[index].get_second_name());
	for (int i = 0; i < users[i].get_size_histories_messaging(); i++) {
		History_messaging cur_history = *(users[index].get_history_messaging(i).get());

		std::shared_ptr<History_messaging> hist = std::make_shared<History_messaging>(cur_history);
		user.get()->set_history_messaging(hist);
	}

	return user;
}

InMemory::~InMemory() {
	delete mtx;
}
