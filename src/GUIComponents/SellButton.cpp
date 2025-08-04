#include "GUIComponents/SellButton.hpp"

#include "Core/JSONLoader.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Gameplay/RadialUpgradeMenu.hpp"
#include "Utility/aligner.hpp"
SellButton::SellButton()
    : ButtonBase(),
      sellIcon{*ResourceManager::getInstance().getTexture("sell_icon")} {
    style.loadJson(JSONLoader::getInstance().getStyle("background_basic"));
    buttonShape.setFillColor(sf::Color::White);
    buttonShape.setOutlineThickness(4);
    buttonShape.setOutlineColor(sf::Color::Black);

    buttonShape.setRadius(35.f);
    buttonShape = Aligner::align(buttonShape, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
    sellIcon = Aligner::align(sellIcon, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
}
void SellButton::update() {
    ButtonBase::update();
    sf::Color fillColor = getFillColor();
    sf::Color borderColor = getBorderColor();

    buttonShape.setFillColor(fillColor);
    buttonShape.setOutlineColor(borderColor);
}

SellButton& SellButton::setPosition(const sf::Vector2f& position) {
    buttonShape.setPosition(position);
    sellIcon.setPosition(position);
    return *this;
}

SellButton& SellButton::setParentRadialMenu(RadialUpgradeMenu* radialMenu) {
    parentRadialMenu = radialMenu;
    return *this;
}

void SellButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(buttonShape, states);
    target.draw(sellIcon, states);
}

bool SellButton::onMouseEvent(Mouse button, UserEvent event,
                              const sf::Vector2f& worldPosition,
                              const sf::Vector2f& windowPosition) {
    if (event == UserEvent::Press && contains(worldPosition) &&
        button == Mouse::Left) {
        if (parentRadialMenu) {
            parentRadialMenu->notify("sell");
        }
        return true;
    }
    return false;
}

bool SellButton::onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                   const sf::Vector2f& windowPosition) {
    return false;
}
bool SellButton::contains(const sf::Vector2f& mousePosition) {
    if (buttonShape.getGlobalBounds().contains(mousePosition)) {
        return true;
    }
    return false;
}