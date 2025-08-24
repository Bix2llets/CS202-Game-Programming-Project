#pragma once
#include "Scene/Overlays/Overlay.hpp"
#include "Core/MouseObserver.hpp"
class WinningScreen : public Overlay {
    private:
    void constructButtons();
    sf::Text message1, message2;

    public:

    WinningScreen(Mediator& mediator);
    ~WinningScreen();

    void update();
    void render() const;

    bool onMouseEvent(Mouse mouse, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override {
        return false; // No scroll event handling in game over screen
    }

};