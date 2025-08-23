#include "GUIComponents/staticSellMenu.hpp"

#include <format>

#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "Gameplay/Currency.hpp"
#include "Scene/Level.hpp"
#include "Utility/logger.hpp"
StaticSellMenu::StaticSellMenu(Level& parentLevel)
    : parentLevel(parentLevel),
      displaying(false),
      priceTag(GameConstants::BLANK_TEXTURE) {
    position = sf::Vector2f(0, 0);
    focusedEntity = nullptr;

    sellButton.setParentMediator(this)
        .setRadius(30)
        .setStyle("borderless_background_basic")
        .setCallback([this](CircularButton* button) { notify("sell"); });
    subscribe("sell", [this](std::any sender, std::any data) {
        try {
            StaticEntity* entity = std::any_cast<StaticEntity*>(focusedEntity);
            Currency cost = entity->getRemoveCost();
            if (this->parentLevel.getBudget().getPetroleum().value <
                    cost.getPetroleum().value ||
                this->parentLevel.getBudget().getScraps().value <
                    cost.getScraps().value) {
                Logger::error("Not enough budget to destroy the entity");
                return;
            }
            this->parentLevel.notify("sell_static_entity", entity);
        } catch (std::bad_any_cast& e) {
            Logger::error(std::format(
                "StaticSellMenu: Bad any cast in sell event handler: {}",
                e.what()));
        }
    });
}

void StaticSellMenu::draw(sf::RenderTarget& target, sf::RenderStates states) {
    if (!displaying || focusedEntity == nullptr) return;

    sellButton.draw(target, states);
    target.draw(priceTag, states);



    
}

void StaticSellMenu::focusOn(StaticEntity* entity) {
    focusedEntity = entity;
    displaying = true;
    position = entity->getPosition();
    sellButton.setPosition(position);
    priceTag.setPosition(position + sf::Vector2f(0, +50));
    priceTag.setTextureRect(sf::IntRect({0, 0}, {0, 0}));
    update();
}

void StaticSellMenu::unFocus() {
    focusedEntity = nullptr;
    displaying = false;
    position = {-1000, -1000};
}

void StaticSellMenu::update() {
    if (focusedEntity == nullptr) return;
    if (parentLevel.getBudget().getPetroleum().value <
            focusedEntity->getRemoveCost().getPetroleum().value ||
        parentLevel.getBudget().getScraps().value <
            focusedEntity->getRemoveCost().getScraps().value) {
        sellButton.setOverlayColor(sf::Color::Red);
    } else {
        sellButton.removeOverlayColor();
    }
    sellButton.update();
    Window::getInstance().toggleUserMode();
    sf::Vector2f worldPosition = static_cast<sf::Vector2f>(
        Window::getInstance().getRenderWindow().mapCoordsToPixel(
            focusedEntity->getPosition()));
    if (worldPosition.x < 0 + sellButton.getRadius()) worldPosition.x = sellButton.getRadius();
    if (worldPosition.x + sellButton.getRadius() >
        GameConstants::MENU_X - sellButton.getRadius()) {
        worldPosition.x = GameConstants::MENU_X - sellButton.getRadius();
    }
    if (worldPosition.y < 0 + sellButton.getRadius()) worldPosition.y = sellButton.getRadius();
    if (worldPosition.y + sellButton.getRadius() >
        GameConstants::DEFAULT_WINDOW_HEIGHT - sellButton.getRadius()) {
        worldPosition.y = GameConstants::DEFAULT_WINDOW_HEIGHT - sellButton.getRadius();
    }
    sellButton.setPosition(worldPosition);
}

bool StaticSellMenu::onMouseEvent(Mouse mouse, UserEvent event,
                                  const sf::Vector2f& worldPosition,
                                  const sf::Vector2f& windowPosition) {
    if (!displaying || focusedEntity == nullptr) return false;

    if (sellButton.onMouseEvent(mouse, event, worldPosition, windowPosition)) {
        return true;
    }
    return false;
}

bool StaticSellMenu::onScrollEvent(float delta,
                                   const sf::Vector2f& worldPosition,
                                   const sf::Vector2f& windowPosition) {
    if (!displaying || focusedEntity == nullptr) return false;

    // Handle scroll events if needed
    return false;
}

void StaticSellMenu::render(sf::RenderStates state) const {
    if (!displaying || focusedEntity == nullptr) return;

    sf::RenderTarget& target = Window::getInstance().getRenderWindow();
    Window::getInstance().toggleGUIMode();
    target.draw(sellButton, state);
    target.draw(priceTag, state);

    
    sf::Vector2f size = focusedEntity->getBounds().size;
    float radius = size.length() / 2;
    
    sf::CircleShape highlightCircle(radius);
    highlightCircle.setOrigin({radius, radius});
    highlightCircle.setPosition(focusedEntity->getPosition());
    highlightCircle.setOutlineColor(sf::Color(116, 122, 118, 255));
    highlightCircle.setOutlineThickness(6.f);
    highlightCircle.setFillColor(sf::Color::Transparent);
    highlightCircle.setPointCount(8);
    Window::getInstance().toggleUserMode();
    target.draw(highlightCircle, state);
}