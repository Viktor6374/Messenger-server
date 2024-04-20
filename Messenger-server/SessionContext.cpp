#define _CRT_SECURE_NO_WARNINGS
#include "SessionContext.h"
#include "SharedContextSingleton.h"
#include "MemoryAccess.h"
#include "InMemory.h"


SessionContext::SessionContext() 
{
	memory_access = InMemory::get_instance();
}

void SessionContext::set_session(std::shared_ptr<Session> session) {
	session_ = session;
}


void SessionContext::set_message(std::string mess) {
	try {
		std::istringstream json_mess(mess);
		boost::property_tree::ptree json_data;

		boost::property_tree::read_json(json_mess, json_data);

		std::string command = json_data.get<std::string>("command");

		if (command == "initialize") {
			initialize(json_data);
		}
		else if (command == "send_message")
		{
			send_message(json_data);
		}
		else if (command == "add_new_user")
		{
			add_new_user(json_data);
		}
	}
	catch (std::exception e) {
		boost::property_tree::ptree json_response;

		json_response.put("command", "error");
		json_response.put("text", e.what());

		std::ostringstream response_string;
		boost::property_tree::write_json(response_string, json_response);

		write_(response_string.str());
	}
}

void SessionContext::set_responce_new_user(std::shared_ptr<User> new_user) {

}

void SessionContext::set_responce_new_message(Message message) {
	boost::property_tree::ptree json_response;

	json_response.put("command", "new_message");
	json_response.put("sender", message.get_sender_username());
	json_response.put("message", message.get_message());

	time_t currentTime = message.get_send_time();

	struct tm* tmStruct = gmtime(&currentTime);

	char iso8601Buffer[25];
	strftime(iso8601Buffer, sizeof(iso8601Buffer), "%Y-%m-%dT%H:%M:%SZ", tmStruct);

	std::string time(iso8601Buffer);

	json_response.put("date_time", time);

	std::ostringstream response_string;
	boost::property_tree::write_json(response_string, json_response);

	session_.get()->doWrite(response_string.str());
}

void SessionContext::set_responce_send_message(Message message) {
	boost::property_tree::ptree json_response;

	json_response.put("command", "send_message");
	json_response.put("sender", message.get_sender_username());
	json_response.put("message", message.get_message());
	json_response.put("addressee", message.get_addresses_username());

	time_t currentTime = message.get_send_time();

	struct tm* tmStruct = gmtime(&currentTime);

	char iso8601Buffer[25];
	strftime(iso8601Buffer, sizeof(iso8601Buffer), "%Y-%m-%dT%H:%M:%SZ", tmStruct);

	std::string time(iso8601Buffer);

	json_response.put("date_time", time);

	std::ostringstream response_string;
	boost::property_tree::write_json(response_string, json_response);

	session_.get()->doWrite(response_string.str());
}

void SessionContext::set_responce_new_chat(std::string interlocutor_username) {
	boost::property_tree::ptree json_response;

	std::shared_ptr<User> interlocutor = memory_access.get()->get_user(interlocutor_username);

	json_response.put("command", "add_interlocutor");
	json_response.put("username", interlocutor_username);
	json_response.put("first_name", interlocutor.get()->get_first_name());
	json_response.put("second_name", interlocutor.get()->get_second_name());

	std::ostringstream response_string;
	boost::property_tree::write_json(response_string, json_response);

	session_.get()->doWrite(response_string.str());
}

void SessionContext::initialize(boost::property_tree::ptree json) {
	std::string login = json.get<std::string>("login");
	std::string password = json.get<std::string>("password");

	SharedContextSingleton * shared_context = SharedContextSingleton::get_instance();

	std::shared_ptr<SessionContext> shared = shared_from_this();
	current_user = shared_context->initialization_session(login, password, shared, memory_access);

	boost::property_tree::ptree json_response;

	json_response.put("command", "initialize");
	json_response.put("username", current_user.get()->get_username());
	json_response.put("first_name", current_user.get()->get_first_name());
	json_response.put("second_name", current_user.get()->get_second_name());

	boost::property_tree::ptree array_node;

	for (int i = 0; i < current_user.get()->get_size_histories_messaging(); i++) {
		boost::property_tree::ptree element_node = parse_history_messaging(current_user.get()->get_history_messaging(i));
		array_node.push_back(std::make_pair("", element_node));
	}

	json_response.put_child("users", array_node);

	std::ostringstream response_string;
	boost::property_tree::write_json(response_string, json_response);

	write_(response_string.str());
}

void SessionContext::write_(std::string str) {
	session_.get()->doWrite(str);
}

void SessionContext::send_message(boost::property_tree::ptree json) {
	std::string sender = json.get<std::string>("sender");
	std::string addresse = json.get<std::string>("addressee");
	std::string message = json.get<std::string>("message");
	std::string time_str = json.get<std::string>("time");

	std::tm tmStruct = {};
	std::istringstream iss(time_str);
	iss >> std::get_time(&tmStruct, "%Y-%m-%dT%H:%M:%SZ");

	std::time_t time = std::mktime(&tmStruct);

	Message mess(sender, addresse, message, time);

	memory_access.get()->save_message(mess);
	current_user.get()->get_history_messaging(addresse).get()->add_message(mess);

	SharedContextSingleton * shared_context = SharedContextSingleton::get_instance();
	shared_context->set_message(mess);
}

void SessionContext::add_new_user(boost::property_tree::ptree json) {
	std::string username = json.get<std::string>("username");
	SharedContextSingleton * shared_context = SharedContextSingleton::get_instance();

	std::shared_ptr<History_messaging> new_hist = std::make_shared<History_messaging>(current_user.get()->get_username(), username);
	current_user.get()->set_history_messaging(new_hist);
	memory_access.get()->save_history_messaging(*(new_hist.get()));

	shared_context->create_history_messaging(current_user.get()->get_username(), username, new_hist);
}

boost::property_tree::ptree SessionContext::parse_history_messaging(std::shared_ptr<History_messaging> hist) {
	std::string interlocutor_username;
	if (hist.get()->get_username_1() == current_user.get()->get_username()) {
		interlocutor_username = hist.get()->get_username_2();
	}
	else
	{
		interlocutor_username = hist.get()->get_username_1();
	}

	std::shared_ptr<User> interlocutor = memory_access.get()->get_user(interlocutor_username);

	boost::property_tree::ptree result_node;
	result_node.put("username", interlocutor_username);
	result_node.put("first_name", interlocutor.get()->get_first_name());
	result_node.put("second_name", interlocutor.get()->get_second_name());

	boost::property_tree::ptree array_node;

	std::vector<Message> messages = hist.get()->get_messages();

	for (int i = 0; i < messages.size(); i++) {
		boost::property_tree::ptree element_node = parse_message(messages[i]);
		array_node.push_back(std::make_pair("", element_node));
	}

	result_node.put_child("messages", array_node);

	return result_node;
}

boost::property_tree::ptree SessionContext::parse_message(Message mess) {
	boost::property_tree::ptree element_node;
	element_node.put("sender_username", mess.get_sender_username());
	element_node.put("message", mess.get_message());

	time_t currentTime = mess.get_send_time();
	struct tm* tmStruct = gmtime(&currentTime);

	char iso8601Buffer[25];
	strftime(iso8601Buffer, sizeof(iso8601Buffer), "%Y-%m-%dT%H:%M:%SZ", tmStruct);

	std::string time(iso8601Buffer);

	element_node.put("send_time", time);

	return element_node;
}

std::shared_ptr<User> SessionContext::get_user()
{
	return current_user;
}

