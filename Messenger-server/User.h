#pragma once
#include <string>
#include <mutex>
#include<map>
#include"History_messaging.h"
#include <memory>

class History_messaging;

class User
{
private:
	std::string username;
	std::string first_name;
	std::string second_name;
	std::vector<std::shared_ptr<History_messaging>> histories_messaging;
	std::mutex * mtx_histories_messaging;

public:
	User(std::string username, std::string first_name, std::string second_name);
	User(const User& other);
	std::string get_username();
	std::string get_first_name();
	std::string get_second_name();
	std::shared_ptr<History_messaging> get_history_messaging(std::string username);
	std::shared_ptr<History_messaging> get_history_messaging(int index);
	std::shared_ptr<History_messaging> set_history_messaging(std::shared_ptr<History_messaging> history);
	std::size_t get_size_histories_messaging();
	bool check_having_history_messaging(std::string username);
	bool operator == (const User& user) const;
	bool operator != (const User& user) const;
	~User();
};

