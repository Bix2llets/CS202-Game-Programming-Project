#pragma once

#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "Entity/Tower/Tower.hpp"
#include "GUIComponents/Mediator.hpp"
#include "GUIComponents/button.hpp"
#include "Gameplay/Currency.hpp"

class TowerMenu : public Mediator , public MouseObserver  {
private:
    std::map<std::string, std::unique_ptr<Tower>> towerList;

    sf::Sprite basePanel;
    std::vector<std::unique_ptr<Button>> towerButtons;
    std::vector<std::unique_ptr<sf::Texture>> combinedTowerTextures;
    std::vector<std::unique_ptr<sf::Texture>> towerTextures;
    std::vector<sf::RenderTexture> renderTexes;
    sf::Vector2f position;

    sf::RectangleShape baseRectangle;

    const sf::Vector2i buttonGap = {5, 5};
    const sf::Vector2i buttonSize = {80, 80};
    const sf::Vector2f borderSize = {5, 5};
    const int BUTTON_PER_ROW = 2;

    const Currency& budgetRef;
    sf::Text petroleumDisplay;
    sf::Text scrapDisplay;

    sf::Sprite scrapIcon;
    sf::Sprite petrolIcon;

    bool isTowerSelected;

    Level* level; // Reference to the level for tower placement

public:
    TowerMenu(const Currency& budget, Level* level);
    void update();

    void render(sf::RenderStates state) const;

    void onLoad();
    void onUnload();

    bool onMouseEvent(Mouse mouse, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition);
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition);

    inline bool contains(sf::Vector2f position) {
        return baseRectangle.getGlobalBounds().contains(position);
    }
private:
    void setResourceDisplay();
    void setTowerButtonDisplay();
    void registerMessages();
};