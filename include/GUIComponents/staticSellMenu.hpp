#pragma once


#include "Entity/StaticEntity/StaticEntity.hpp"
#include "GUIComponents/Mediator.hpp"
#include "GUIComponents/CircularButton.hpp"
#include "Core/MouseObserver.hpp"
#include <SFML/Graphics.hpp>

class Level;
class StaticSellMenu : public MouseObserver, public Mediator{
    private:
    Level& parentLevel;
    bool displaying;
    sf::Vector2f position;

    CircularButton sellButton;
    sf::Sprite priceTag;
    StaticEntity* focusedEntity = nullptr;
    public:
    StaticSellMenu(Level& parentLevel);

    void draw(sf::RenderTarget& target, sf::RenderStates states);
    void focusOn(StaticEntity* entity);
    void unFocus();

    void render(sf::RenderStates state) const;
    inline bool isDisplaying() const { return displaying; }
    bool onMouseEvent(Mouse button, UserEvent event,
                      const sf::Vector2f& worldPosition,
                      const sf::Vector2f& windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f& worldPosition, const sf::Vector2f& windowPosition) override;

    void update();

};