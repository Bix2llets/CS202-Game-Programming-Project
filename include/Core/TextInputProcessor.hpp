#pragma once
#include <SFML/Graphics.hpp>
#include "Core/TextInputObserver.hpp"
#include <list>
class TextInputProcessor {
    private:
    
    std::list<TextInputObserver*> observerList;
    
    public:
    
    // * Disable for singleton
    
    TextInputProcessor() = default;
    void handleEvent(const std::optional<sf::Event>& event);

    void addSubscriber(TextInputObserver& observer);
    void removeSubscriber(TextInputObserver& observer);

    inline void clearSubscribers() {observerList.clear();}
};

