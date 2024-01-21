#pragma once
#include "MemoryAccess.h"
#include <Mutex>

class SessionContext
{
public:
	SessionContext();
	void set_message(std::string mess);
	void set_responce_new_user(User new_user);
	void set_responce_new_message(Message message);
	void set_responce_send_message(Message message);
	void set_responce_new_chat(User interlocutor);
private:
	std::shared_ptr<MemoryAccess> memory_access;
	std::shared_ptr<User> current_user;
	std::mutex mtx;
};

