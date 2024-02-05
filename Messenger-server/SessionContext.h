#pragma once
#include "MemoryAccess.h"
#include "Session.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <Mutex>

class Session;

class SessionContext : public std::enable_shared_from_this<SessionContext>
{
public:
	SessionContext();
	void set_session(std::shared_ptr<Session> session);
	void set_message(std::string mess);
	void set_responce_new_user(std::shared_ptr<User> new_user);
	void set_responce_new_message(Message message);
	void set_responce_send_message(Message message);
	void set_responce_new_chat(std::string interlocutor_username);
	std::shared_ptr<User> get_user();
private:
	std::shared_ptr<MemoryAccess> memory_access;
	std::shared_ptr<User> current_user;
	std::shared_ptr<Session> session_;
	void initialize(boost::property_tree::ptree json);
	void send_message(boost::property_tree::ptree json);
	void add_new_user(boost::property_tree::ptree json);
	boost::property_tree::ptree parse_history_messaging(std::shared_ptr<History_messaging> hist);
	boost::property_tree::ptree parse_message(Message mess);
};

