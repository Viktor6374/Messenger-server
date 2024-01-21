#pragma once
#include "User.h"

class MemoryAccess
{
	virtual void save_new_user(User new_user);
	virtual void save_message(Message message);
	virtual void save_history_messaging(History_messaging history);
	virtual std::shared_ptr<User> get_user(std::string username, std::string password);
};

