#pragma once
#include <string>
#include <mutex>
#include<map>
#include"History_messaging.h"
#include <memory>

class User
{
private:
	std::string username;
	std::string first_name;
	std::string second_name;
	std::map<std::string, std::shared_ptr<History_messaging>> histories_messaging;
	std::mutex mtx_histories_messaging;

public:
	User(std::string username, std::string first_name, std::string second_name);
	std::string get_username();
	std::string get_first_name();
	std::string get_second_name();
	std::shared_ptr<History_messaging> get_history_messaging(User& user);
	std::shared_ptr<History_messaging> set_history_messaging(std::shared_ptr<History_messaging> history);
	bool operator == (const User& user) const;
	bool operator != (const User& user) const;
};

