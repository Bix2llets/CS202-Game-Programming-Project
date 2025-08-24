#include "GUIComponents/staticSellMenu.hpp"

#include <format>

#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "Gameplay/Currency.hpp"
#include "Scene/Level.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
StaticSellMenu::StaticSellMenu(Level& parentLevel)
    : parentLevel(parentLevel),
      displaying(false),
      priceTag(GameConstants::BLANK_TEXTURE),
      priceTagSprite(GameConstants::BLANK_TEXTURE) {
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
    updateSpriteTag();
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
    if (worldPosition.x < 0 + sellButton.getRadius())
        worldPosition.x = sellButton.getRadius();
    if (worldPosition.x + sellButton.getRadius() >
        GameConstants::MENU_X - sellButton.getRadius()) {
        worldPosition.x = GameConstants::MENU_X - sellButton.getRadius();
    }
    if (worldPosition.y < 0 + sellButton.getRadius())
        worldPosition.y = sellButton.getRadius();
    if (worldPosition.y + sellButton.getRadius() >
        GameConstants::DEFAULT_WINDOW_HEIGHT - sellButton.getRadius()) {
        worldPosition.y =
            GameConstants::DEFAULT_WINDOW_HEIGHT - sellButton.getRadius();
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
    // Window::getInstance().toggleGUIMode();
    target.draw(priceTag, state);
}

void StaticSellMenu::updateSpriteTag() {
    if (focusedEntity == nullptr) return;
    Currency cost = focusedEntity->getRemoveCost();
    std::string priceText = "Destroy";
    std::string petroleumPart = std::format("{}", cost.getPetroleum().value);
    std::string scrapPart = std::format("{}", cost.getScraps().value);

    const sf::Font* font = ResourceManager::getInstance().getFont("text");
    sf::Text text(*font);
    sf::Text text1(*font);
    sf::Text text2(*font);
    auto configText = [](sf::Text& text, std::string content) {
        text.setString(content);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2);
        text.setLineSpacing(1.5f);
    };
    configText(text, priceText);
    configText(text1, scrapPart);
    configText(text2, petroleumPart);

    Aligner::align(text, HorizontalAlignment::Left,
                   VerticalAlignment::Middle);
    Aligner::align(text1, HorizontalAlignment::Left,
                   VerticalAlignment::Middle);
    Aligner::align(text2, HorizontalAlignment::Left,
                   VerticalAlignment::Middle);

    if (!priceTagTexture.resize(
            {static_cast<unsigned int>(text.getLocalBounds().size.x + 20),
             static_cast<unsigned int>(text.getLocalBounds().size.y + 20 + text1.getLocalBounds().size.y + text2.getLocalBounds().size.y + 5 + 5)})) {
        Logger::error("StaticSellMenu: Failed to create price tag texture");
        return;
    }

    text.setPosition(
        {10, 0 + 20});
    text1.setPosition({30,
                       text.getPosition().y + text.getGlobalBounds().size.y / 2.f + text1.getGlobalBounds().size.y / 2.f + 5});
    text2.setPosition({30,
                       text1.getPosition().y + text1.getGlobalBounds().size.y / 2.f + text2.getGlobalBounds().size.y / 2.f + 5});
    priceTagTexture.clear(sf::Color(0, 0, 0, 100));
    priceTagTexture.draw(text);
    priceTagTexture.draw(text1);
    priceTagTexture.draw(text2);

    sf::Sprite petroleumIcon(
        *ResourceManager::getInstance().getTexture("petroleum"));
    sf::Sprite scrapIcon(
        *ResourceManager::getInstance().getTexture("scrap"));
    petroleumIcon = Scaler::scaleSprite(petroleumIcon, {20, 20});
    scrapIcon = Scaler::scaleSprite(scrapIcon, {20, 20});
    petroleumIcon.setPosition({5, text2.getPosition().y - 10});
    scrapIcon.setPosition({5, text1.getPosition().y - 10});
    priceTagTexture.draw(petroleumIcon);
    priceTagTexture.draw(scrapIcon);
    priceTagTexture.display();

    priceTag = sf::Sprite(priceTagTexture.getTexture());
    Aligner::align(priceTag, HorizontalAlignment::Center,
                   VerticalAlignment::Top);
    priceTag.setPosition(position + sf::Vector2f(0, +45));
}