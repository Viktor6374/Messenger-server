#pragma once
#include "MemoryAccess.h"
#include "SessionContext.h"
#include <mutex>

class InMemory : public MemoryAccess
{
public:
	void save_new_user(User new_user, std::string password) override;
	void save_message(Message message) override;
	void save_history_messaging(History_messaging history) override;
	std::shared_ptr<User> initialize_user(std::string username, std::string password) override;
	std::shared_ptr<User> get_user(std::string username) override;


	static std::shared_ptr<InMemory> get_instance();
	~InMemory();
private:
	InMemory();
	std::vector<User> users;
	std::map<std::string, std::string> logins_and_passwords;
	std::shared_ptr<User> create_user(int index);
	std::recursive_mutex * mtx;


	static std::shared_ptr<InMemory> instance;
	static boost::mutex * static_mtx;
	void init();
};

