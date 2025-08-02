#include "GUIComponents/UpgradeButton.hpp"

#include "Base/Constants.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
UpgradeButton::UpgradeButton()
    : radius(0),
      parentRadialMenu(nullptr),
      upgradeIcon(GameConstants::BLANK_TEXTURE),
      tagDisplay(GameConstants::BLANK_TEXTURE) {
    buttonShape.setFillColor(sf::Color::Transparent);
    buttonShape.setOutlineThickness(2);
    buttonShape.setOutlineColor(sf::Color::White);

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
    buttonShape.setOrigin({radius, radius});
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
    scrapText = Aligner::align(scrapText, HorizontalAlignment::Left, VerticalAlignment::Middle);
    petrolIcon = Aligner::align(petrolIcon, HorizontalAlignment::Center, VerticalAlignment::Middle);
    scrapIcon = Aligner::align(scrapIcon, HorizontalAlignment::Center, VerticalAlignment::Middle);

    sf::RenderTexture priceContent;
    const int iconTextPadding = 5;
    const int contentPadding = 10;
    sf::Vector2u contentDimension;
    contentDimension.x = petrolIcon.getLocalBounds().size.x + 
                         scrapIcon.getLocalBounds().size.x + 
                         petroleumText.getLocalBounds().size.x + 
                         scrapText.getLocalBounds().size.x + 
                         iconTextPadding * 2 + contentPadding * 3;
    contentDimension.y = std::max(petrolIcon.getLocalBounds().size.y, petroleumText.getLocalBounds().size.y) + contentPadding * 2;
    priceContent.resize(contentDimension);

    priceContent.clear(sf::Color::Transparent);
    int baselineY = contentDimension.y / 2;
    scrapIcon.setPosition({contentPadding, baselineY});
    scrapText.setPosition({scrapIcon.getPosition().x + scrapIcon.getLocalBounds().size.x / 2 + iconTextPadding, scrapIcon.getPosition().y});
    petrolIcon.setPosition({scrapText.getPosition().x + scrapText.getLocalBounds().size.x + contentPadding, scrapText.getPosition().y});
    petroleumText.setPosition({petrolIcon.getPosition().x + petrolIcon.getLocalBounds().size.x / 2 + iconTextPadding, petrolIcon.getPosition().y});
    priceContent.draw(scrapIcon);
    priceContent.draw(scrapText);
    priceContent.draw(petrolIcon);
    priceContent.draw(petroleumText);
    priceContent.display();

    sf::Sprite priceSprite(priceContent.getTexture());
    priceSprite = Aligner::align(priceSprite, HorizontalAlignment::Center, VerticalAlignment::Middle);

    int borderThickness = 2;
    priceTag.resize(static_cast<sf::Vector2u>(priceSprite.getLocalBounds().size + sf::Vector2f{borderThickness, borderThickness}));
    priceTag.clear(sf::Color::Transparent);
    priceSprite.setPosition(static_cast<sf::Vector2f>(priceTag.getSize()) / 2.f);
    sf::RectangleShape background;
    background.setPosition({borderThickness, borderThickness});
    background.setSize(static_cast<sf::Vector2f>(priceTag.getSize() - sf::Vector2u{borderThickness * 2, borderThickness * 2}));
    background.setFillColor(sf::Color::White);
    background.setOutlineThickness(borderThickness);
    background.setOutlineColor(sf::Color::White);
    priceTag.draw(background);
    priceTag.draw(priceSprite);
    priceTag.display();
    tagDisplay.setTexture(priceTag.getTexture());
    tagDisplay.setPosition(buttonShape.getPosition() + sf::Vector2f{9, radius} * 9.f / 10.f);



}