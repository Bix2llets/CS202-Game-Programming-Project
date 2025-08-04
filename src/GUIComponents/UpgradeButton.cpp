#include "GUIComponents/UpgradeButton.hpp"

#include "Base/Constants.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Gameplay/RadialUpgradeMenu.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
#include "Core/MouseState.hpp"
#include "Core/UserEvent.hpp"
UpgradeButton::UpgradeButton()
    : radius(0),
      parentRadialMenu(nullptr),
      upgradeIcon(GameConstants::BLANK_TEXTURE),
      tagDisplay(GameConstants::BLANK_TEXTURE) {
    buttonShape.setFillColor(sf::Color::White);
    buttonShape.setOutlineThickness(4);
    buttonShape.setOutlineColor(sf::Color::Black);

    buttonShape= Aligner::align(buttonShape, HorizontalAlignment::Center,
                                   VerticalAlignment::Middle);

    
    style.loadJson(JSONLoader::getInstance().getStyle("background_basic"));
    price.setPetroleum(0);
    price.setScraps(0);
}
UpgradeButton& UpgradeButton::setPosition(const sf::Vector2f& position) {
    buttonShape.setPosition(position);
    return *this;
}

UpgradeButton& UpgradeButton::setRadius(int radius) {
    this->radius = radius;
    buttonShape.setRadius(static_cast<float>(radius));
    buttonShape.setOrigin(
        {static_cast<float>(radius), static_cast<float>(radius)});
    return *this;
}

void UpgradeButton::updatePriceTag() {
    sf::Text petroleumText(*style.getFont());
    sf::Text scrapText(*style.getFont());

    sf::Sprite petrolIcon(price.getPetroleum().icon);
    sf::Sprite scrapIcon(price.getScraps().icon);

    petroleumText.setString(std::to_string(price.getPetroleum().value));
    scrapText.setString(std::to_string(price.getScraps().value));

    petroleumText.setCharacterSize(12);
    scrapText.setCharacterSize(12);

    petroleumText.setFillColor(sf::Color::White);
    scrapText.setFillColor(sf::Color::White);

    petroleumText = Aligner::align(petroleumText, HorizontalAlignment::Left,
                                   VerticalAlignment::Middle);
    scrapText = Aligner::align(scrapText, HorizontalAlignment::Left,
                               VerticalAlignment::Middle);
    petrolIcon = Aligner::align(petrolIcon, HorizontalAlignment::Center,
                                VerticalAlignment::Middle);
    scrapIcon = Aligner::align(scrapIcon, HorizontalAlignment::Center,
                               VerticalAlignment::Middle);

    sf::RenderTexture priceContent;
    const int iconTextPadding = 5;
    const int contentPadding = 10;
    sf::Vector2u contentDimension;
    contentDimension.x = petrolIcon.getLocalBounds().size.x +
                         scrapIcon.getLocalBounds().size.x +
                         petroleumText.getLocalBounds().size.x +
                         scrapText.getLocalBounds().size.x +
                         iconTextPadding * 2 + contentPadding * 3;
    contentDimension.y = std::max(petrolIcon.getLocalBounds().size.y,
                                  petroleumText.getLocalBounds().size.y) +
                         contentPadding * 2;
    bool resizeResult = priceContent.resize(contentDimension);

    if (resizeResult == false) {
        Logger::error("Failed to resize price content texture");
        return;
    }

    priceContent.clear(sf::Color::Transparent);
    int baselineY = contentDimension.y / 2;
    scrapIcon.setPosition({contentPadding, static_cast<float>(baselineY)});
    scrapText.setPosition({scrapIcon.getPosition().x +
                               scrapIcon.getLocalBounds().size.x / 2 +
                               iconTextPadding,
                           scrapIcon.getPosition().y});
    petrolIcon.setPosition({scrapText.getPosition().x +
                                scrapText.getLocalBounds().size.x +
                                contentPadding,
                            scrapText.getPosition().y});
    petroleumText.setPosition({petrolIcon.getPosition().x +
                                   petrolIcon.getLocalBounds().size.x / 2 +
                                   iconTextPadding,
                               petrolIcon.getPosition().y});
    priceContent.draw(scrapIcon);
    priceContent.draw(scrapText);
    priceContent.draw(petrolIcon);
    priceContent.draw(petroleumText);
    priceContent.display();

    sf::Sprite priceSprite(priceContent.getTexture());
    priceSprite = Aligner::align(priceSprite, HorizontalAlignment::Center,
                                 VerticalAlignment::Middle);

    float borderThickness = 2.f;
    if (!priceTag.resize(static_cast<sf::Vector2u>(
            priceSprite.getLocalBounds().size +
            sf::Vector2f{borderThickness, borderThickness}))) {
        Logger::error("Failed to resize price tag texture");
        throw std::runtime_error("Failed to resize price tag texture");
    }
    priceTag.clear(sf::Color::Transparent);
    priceSprite.setPosition(static_cast<sf::Vector2f>(priceTag.getSize()) /
                            2.f);
    sf::RectangleShape background;
    background.setPosition({borderThickness, borderThickness});
    background.setSize(static_cast<sf::Vector2f>(
        priceTag.getSize() -
        sf::Vector2u{static_cast<unsigned int>(borderThickness) * 2,
                     static_cast<unsigned int>(borderThickness) * 2}));
    background.setFillColor(sf::Color::White);
    background.setOutlineThickness(borderThickness);
    background.setOutlineColor(sf::Color::White);
    priceTag.draw(background);
    priceTag.draw(priceSprite);
    priceTag.display();
    tagDisplay.setTexture(priceTag.getTexture());
    tagDisplay.setPosition(buttonShape.getPosition() +
                           sf::Vector2f{9, static_cast<float>(radius)} * 9.f /
                               10.f);
}

void UpgradeButton::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    target.draw(buttonShape, states);
    target.draw(upgradeIcon, states);
    target.draw(tagDisplay, states);
}

void UpgradeButton::update() {
    ButtonBase::update();

    sf::Color fillColor = getFillColor();
    sf::Color borderColor = getBorderColor();
    sf::Color textColor = getTextColor();
    if (!canUpgrade) {
        sf::Color mixColor = sf::Color::Red;
        fillColor = ColorMixer::perceptualLerp(fillColor, mixColor, 0.5f);
        borderColor = ColorMixer::perceptualLerp(borderColor, mixColor, 0.5f);
        textColor = ColorMixer::perceptualLerp(textColor, mixColor, 0.5f);
    }
    buttonShape.setFillColor(fillColor);
    buttonShape.setOutlineColor(borderColor);
    tagDisplay.setColor(fillColor);
    upgradeIcon.setColor(fillColor);
}

void UpgradeButton::refreshInfo() {
    if (parentRadialMenu) {
        price = parentRadialMenu->getUpgradeDetail(upgradeID)->cost;
    } else {
        Logger::error("Parent radial menu is not set for UpgradeButton");
        return;
    }
    updatePriceTag();
}

UpgradeButton& UpgradeButton::setUpgradeID(int id) {
    upgradeID = id;
    return *this;
}

UpgradeButton& UpgradeButton::setPrice(Currency currency) {
    price = currency;
    return *this;
}

UpgradeButton& UpgradeButton::setParentRadialMenu(
    RadialUpgradeMenu* radialMenu) {
    parentRadialMenu = radialMenu;
    return *this;
}

bool UpgradeButton::contains(const sf::Vector2f& mousePosition) {
    if ((mousePosition - buttonShape.getPosition()).length() <= radius)
        return true;
    if (tagDisplay.getGlobalBounds().contains(mousePosition)) return true;
    return false;
}

bool UpgradeButton::onMouseEvent(Mouse button, UserEvent event,
                                 const sf::Vector2f& worldPosition,
                                 const sf::Vector2f& windowPosition) {
    if (event == UserEvent::Press && button == Mouse::Left) {
        if (contains(worldPosition)) {
            updatePressState(true);
            return true;
        }
    } else if (event == UserEvent::Release && button == Mouse::Left) {
        if (isPressed && contains(worldPosition)) {
            updatePressState(false);
            if (canUpgrade) {
                parentRadialMenu->notify("upgrade", upgradeID);
            }
            return true;
        }
    } else if (event == UserEvent::Move) {
        if (contains(worldPosition)) {
            updateHoverState(true);
            return true;
        } else {
            updateHoverState(false);
        }
    }
    return false;
}

bool UpgradeButton::onScrollEvent(float delta,
                                  const sf::Vector2f& worldPosition,
                                  const sf::Vector2f& windowPosition) {
    return false;
}

UpgradeButton::UpgradeButton(const UpgradeButton& other)
    : ButtonBase(other),
      radius(other.radius),
      parentRadialMenu(other.parentRadialMenu),
      upgradeID(other.upgradeID),
      canUpgrade(other.canUpgrade),
      price(other.price),
      buttonShape(other.buttonShape),
      upgradeIcon(other.upgradeIcon),
      tagDisplay(other.tagDisplay) {
    // Deep copy for priceTag (sf::RenderTexture)
    // SFML RenderTexture cannot be copied directly, so we copy the texture if available
    updatePriceTag();
}

void UpgradeButton::setCanUpgrade(bool canUpgrade) {
    this->canUpgrade = canUpgrade;
    update();
}