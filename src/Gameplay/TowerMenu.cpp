#include "Gameplay/TowerMenu.hpp"

#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "Gameplay/Currency.hpp"
TowerMenu::TowerMenu(const Currency& currencyRef)
    : basePanel{*ResourceManager::getInstance().getTexture(
          "tower_selection_base")},
      budgetRef{currencyRef},
      petroleumDisplay{*ResourceManager::getInstance().getFont("pixel")},
      scrapDisplay{*ResourceManager::getInstance().getFont("pixel")},
      scrapIcon{currencyRef.getScraps().icon},
      petrolIcon{currencyRef.getPetroleum().icon} {
    petroleumDisplay.setString("0");
    scrapDisplay.setString("0");
    auto fixOrigin = [](sf::Text& text) {
        text.setOrigin({0.f, (text.getLocalBounds().position.y +
                              text.getLocalBounds().size.y / 2)});
    };

    auto fixOriginIcon = [](sf::Sprite& sprite) {
        sprite.setOrigin(
            (sprite.getLocalBounds().position + sprite.getLocalBounds().size) /
            2.f);
    };
    fixOrigin(petroleumDisplay);
    fixOrigin(scrapDisplay);

    fixOriginIcon(scrapIcon);
    fixOriginIcon(petrolIcon);

    scrapIcon.setPosition(position + sf::Vector2f{40, 40});
    scrapDisplay.setPosition(position + sf::Vector2f{55, 40});
    petrolIcon.setPosition(position + sf::Vector2f{40, 80});
    petroleumDisplay.setPosition(position + sf::Vector2f{55, 80});


    petroleumDisplay.setFillColor(sf::Color::Black);
    scrapDisplay.setFillColor(sf::Color::Black);
    basePanel.setPosition(position);
    basePanel.setScale({size.x / basePanel.getLocalBounds().size.x,
                        size.y / basePanel.getLocalBounds().size.y});
}

void TowerMenu::update() {
    scrapDisplay.setString(std::to_string(budgetRef.getScraps().value));
    petroleumDisplay.setString(std::to_string(budgetRef.getPetroleum().value));
}
void TowerMenu::render(sf::RenderStates state) const {
    sf::RenderWindow& window = Window::getInstance().getRenderWindow();
    window.draw(basePanel, state);

    window.draw(scrapDisplay, state);
    window.draw(petroleumDisplay, state);
    window.draw(scrapIcon, state);
    window.draw(petrolIcon, state);

    auto getCircPos = [](sf::Transformable obj) {
        sf::CircleShape circ;
        circ.setRadius(3);
        circ.setOrigin({3, 3});
        circ.setPosition(obj.getPosition());
        circ.setFillColor(sf::Color::White);
        return circ;
    };
    window.draw(getCircPos(scrapDisplay), state);
    window.draw(getCircPos(petroleumDisplay), state);
    window.draw(getCircPos(scrapIcon), state);
    window.draw(getCircPos(petrolIcon), state);
}