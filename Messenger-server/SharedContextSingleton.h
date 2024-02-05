#pragma once
#include <mutex>
#include "History_messaging.h"
#include "Message.h"
#include "SessionContext.h"


class SharedContextSingleton
{
public:
	static SharedContextSingleton* get_instance();
//	std::shared_ptr<History_messaging> get_history_messaging_by_username(std::string username);
	void set_message(Message message);
	void set_session(std::shared_ptr<SessionContext> session);
	void create_history_messaging(std::string username1, std::string username2);
	std::shared_ptr<User> initialization_session(std::string username
		, std::string password, std::shared_ptr<SessionContext> new_context, std::shared_ptr<MemoryAccess> mem_access);
//	std::shared_ptr<User> create_new_user(std::string username, std::string first_name, std::string second_name, std::string password);
private:
	SharedContextSingleton();
	static SharedContextSingleton* instance;
	static std::mutex mtx;
	std::vector<std::shared_ptr<SessionContext>> sessions;
	std::mutex* mtx_data;
};

