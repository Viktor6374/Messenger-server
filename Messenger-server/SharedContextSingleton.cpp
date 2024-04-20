#include "SharedContextSingleton.h"


SharedContextSingleton* SharedContextSingleton::instance = nullptr;
std::mutex SharedContextSingleton::mtx;

SharedContextSingleton* SharedContextSingleton::get_instance(){
    if (instance == nullptr) {
        std::unique_lock<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new SharedContextSingleton();
        }
    }
    return instance;
}

//std::shared_ptr<History_messaging> SharedContextSingleton::get_history_messaging_by_username(std::string username) {}

void SharedContextSingleton::set_message(Message message) {
    std::lock_guard<std::mutex> guard(*mtx_data);
    for (int i = 0; i < sessions.size(); i++) {
        std::string cur_username = sessions[i].get()->get_user().get()->get_username();
        if (cur_username == message.get_sender_username()) {
            sessions[i].get()->set_responce_send_message(message);
        }
        if (cur_username == message.get_addresses_username()) {
            sessions[i].get()->set_responce_new_message(message);
        }
    }
}

void SharedContextSingleton::set_session(std::shared_ptr<SessionContext> session) {
    std::lock_guard<std::mutex> guard(*mtx_data);
    sessions.push_back(session);
}


SharedContextSingleton::SharedContextSingleton() {
    mtx_data = new std::mutex;
}

void SharedContextSingleton::create_history_messaging(std::string chat_creator, std::string username2, std::shared_ptr<History_messaging> hist){
    std::lock_guard<std::mutex> guard(*mtx_data);
    for (int i = 0; i < sessions.size(); i++) {
        std::string cur_username = sessions[i].get()->get_user().get()->get_username();
        if (cur_username == chat_creator) {
            sessions[i].get()->set_responce_new_chat(username2);
        }
        if (cur_username == username2) {
            sessions[i].get()->get_user().get()->set_history_messaging(hist);
            sessions[i].get()->set_responce_new_chat(chat_creator);
        }
    }
}

std::shared_ptr<User> SharedContextSingleton::initialization_session(std::string username
    , std::string password, std::shared_ptr<SessionContext> context, std::shared_ptr<MemoryAccess> mem_access)
{
    std::lock_guard<std::mutex> guard(*mtx_data);
    std::shared_ptr<User> user = mem_access.get()->initialize_user(username, password);
    for (int i = 0; i < sessions.size(); i++) {
        SessionContext* ses = sessions[i].get();
        User* us = ses->get_user().get();
        User cur_user = *(us);
        if (cur_user.check_having_history_messaging(username)) {
            user.get()->set_history_messaging(cur_user.get_history_messaging(username));
        }
    }
    sessions.push_back(context);

    return user;
}

//std::shared_ptr<User> SharedContextSingleton::create_new_user(std::string username, std::string first_name, std::string second_name, std::string password) {}

std::vector<std::shared_ptr<SessionContext>> SharedContextSingleton::get_sessions(){
    return sessions;
}