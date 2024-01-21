#include "SharedContextSingleton.h"


SharedContextSingleton& SharedContextSingleton::get_instance(){
    if (instance == nullptr) {
        std::unique_lock<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new SharedContextSingleton();
        }
    }
    return *instance;
}

std::shared_ptr<History_messaging> SharedContextSingleton::get_history_messaging_by_username(std::string username) {

}

void SharedContextSingleton::set_message(Message message) {

}

void SharedContextSingleton::set_session(std::shared_ptr<SessionContext> session) {
    sessions.push_back(session);
}


SharedContextSingleton::SharedContextSingleton() {}