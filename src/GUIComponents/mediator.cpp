#include "GUIComponents/mediator.hpp"

void Mediator::subscribe(const std::string& event, Handler handler) {
    handlers[event].push_back(std::move(handler));
}

void Mediator::notify(const std::string& event, std::any sender, std::any data) {
    auto it = handlers.find(event);
    if (it != handlers.end()) {
        for (auto& handler : it->second) {
            handler(sender, data);
        }
    }
}
