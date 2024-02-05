#pragma once
#include "User.h"

class MemoryAccess
{
public:
	virtual void save_new_user(User new_user, std::string password) = 0;
	virtual void save_message(Message message) = 0;
	virtual void save_history_messaging(History_messaging history) = 0;
	virtual std::shared_ptr<User> initialize_user(std::string username, std::string password) = 0;
	virtual std::shared_ptr<User> get_user(std::string username) = 0;
};

