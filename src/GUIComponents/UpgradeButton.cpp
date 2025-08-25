#include "GUIComponents/UpgradeButton.hpp"

#include "Base/Constants.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "Gameplay/RadialUpgradeMenu.hpp"
#include "Scene/Level.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
#include "GUIComponents/cursor.hpp"
UpgradeButton::UpgradeButton()
    : radius(0),
      parentMediator(nullptr),
      upgradeIcon(GameConstants::BLANK_TEXTURE),
      tagDisplay(GameConstants::BLANK_TEXTURE) {
    buttonShape.setFillColor(sf::Color::White);
    buttonShape.setOutlineThickness(8);
    buttonShape.setOutlineColor(sf::Color::Black);

    buttonShape = Aligner::align(buttonShape, HorizontalAlignment::Center,
                                 VerticalAlignment::Middle);

    price.setPetroleum(0);
    price.setScraps(0);
}
UpgradeButton& UpgradeButton::setPosition(const sf::Vector2f& position) {
    buttonShape.setPosition(position);

    tagDisplay.setPosition(
        position + sf::Vector2f{0, static_cast<float>(radius)} * TAG_OFFSET);
    upgradeIcon.setPosition(position);
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
    sf::Text petroleumText(*graphicState.getStyle().getFont());
    sf::Text scrapText(*graphicState.getStyle().getFont());

    sf::Sprite petrolIcon(price.getPetroleum().icon);
    sf::Sprite scrapIcon(price.getScraps().icon);

    petroleumText.setString(std::to_string(price.getPetroleum().value));
    scrapText.setString(std::to_string(price.getScraps().value));

    petroleumText.setCharacterSize(graphicState.getStyle().getFontSize());
    scrapText.setCharacterSize(graphicState.getStyle().getFontSize());

    petroleumText.setFillColor(graphicState.getTextColor());
    scrapText.setFillColor(graphicState.getTextColor());

    scrapIcon.setScale({0.5f, 0.5f});
    petrolIcon.setScale({0.5f, 0.5f});

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
    contentDimension.x = std::max(petrolIcon.getGlobalBounds().size.x +
                                      petroleumText.getGlobalBounds().size.x,
                                  scrapText.getGlobalBounds().size.x +
                                      scrapIcon.getGlobalBounds().size.x) +
                         iconTextPadding * 1 + contentPadding * 2;
    contentDimension.y = scrapText.getGlobalBounds().size.y +
                         petroleumText.getGlobalBounds().size.y +
                         contentPadding * 3;
    bool resizeResult = priceContent.resize(contentDimension);

    if (resizeResult == false) {
        Logger::error("Failed to resize price content texture");
        return;
    }

    priceContent.clear(sf::Color::Transparent);
    int baselineY = contentDimension.y / 2;
    scrapIcon.setPosition(
        {contentPadding + scrapIcon.getGlobalBounds().size.x / 2.f,
         contentPadding + scrapIcon.getGlobalBounds().size.y / 2.f});
    scrapText.setPosition({scrapIcon.getPosition().x +
                               scrapIcon.getGlobalBounds().size.x / 2 +
                               iconTextPadding,
                           scrapIcon.getPosition().y});
    petrolIcon.setPosition(
        {scrapIcon.getPosition().x,
         scrapIcon.getPosition().y + scrapIcon.getGlobalBounds().size.y / 2 +
             contentPadding + petrolIcon.getGlobalBounds().size.y / 2});
    petroleumText.setPosition({petrolIcon.getPosition().x +
                                   petrolIcon.getGlobalBounds().size.x / 2 +
                                   iconTextPadding,
                               petrolIcon.getPosition().y});

    if (!isCapped) {
        priceContent.draw(scrapIcon);
        priceContent.draw(scrapText);
        priceContent.draw(petrolIcon);
        priceContent.draw(petroleumText);
    }
    priceContent.display();

    sf::Sprite priceSprite(priceContent.getTexture());
    priceSprite = Aligner::align(priceSprite, HorizontalAlignment::Center,
                                 VerticalAlignment::Middle);

    float borderThickness = buttonShape.getOutlineThickness();
    if (!priceTag.resize(static_cast<sf::Vector2u>(
            priceSprite.getGlobalBounds().size +
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
    background.setFillColor(graphicState.getFillColor());
    background.setOutlineThickness(borderThickness);
    background.setOutlineColor(graphicState.getBorderColor());
    priceTag.draw(background);
    priceTag.draw(priceSprite);
    priceTag.display();
    // priceTag.setSmooth(true);
    tagDisplay = sf::Sprite(priceTag.getTexture());
    tagDisplay = Aligner::align(tagDisplay, HorizontalAlignment::Center,
                                VerticalAlignment::Top);
    tagDisplay.setPosition(buttonShape.getPosition() +
                           sf::Vector2f{0, static_cast<float>(radius)} *
                               TAG_OFFSET);
}

void UpgradeButton::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    int level = upgrades->getUpgradeType(upgradeID) == nullptr
                    ? 0
                    : upgrades->getCurrentLevel(upgradeID);

    for (int i = 1; i <= level; i++) {
        sf::CircleShape levelIndicator;
        levelIndicator.setRadius(radius + buttonShape.getOutlineThickness() + i * 4);
        levelIndicator.setOrigin(
            {levelIndicator.getRadius(), levelIndicator.getRadius()});
        levelIndicator.setPosition(buttonShape.getPosition());
        levelIndicator.setFillColor(sf::Color::Transparent);
        levelIndicator.setOutlineThickness(3);
        levelIndicator.setOutlineColor(sf::Color::White);
        target.draw(levelIndicator, states);
    }
    target.draw(buttonShape, states);
    if (!isCapped || evolutionTower) {
        target.draw(upgradeIcon, states);
        target.draw(tagDisplay, states);
    }
}

void UpgradeButton::update() {
    ButtonBase::update();

    if (!canUpgrade && !isCapped && !evolutionTower) {
        graphicState.setOverlayColor(sf::Color::Red);   
    } else {
        graphicState.removeOverlayColor();  
    }
    sf::Color fillColor = graphicState.getFillColor();
    sf::Color borderColor = graphicState.getBorderColor();
    sf::Color textColor = graphicState.getTextColor();
    updatePriceTag();
    buttonShape.setFillColor(fillColor);
    buttonShape.setOutlineColor(borderColor);
    upgradeIcon.setColor(sf::Color(255, 255, 255, fillColor.a));
}

void UpgradeButton::refreshInfo() {
    if (upgrades->getNextUpgradeDetail(upgradeID) == nullptr) {
        isCapped = true;
        price = Currency(0, 0);
        upgradeIcon.setTexture(GameConstants::BLANK_TEXTURE);
        upgradeIcon.setTextureRect(sf::IntRect({0, 0}, {0, 0}));
        if (upgrades->canEvolve(upgradeID)) {
            Scene& parentScene = parentMediator->getParentLevel();
            evolutionTower = TowerFactory::createFromConfigFile(
                upgrades->getEvolveTo(upgradeID),
                static_cast<Scene&>(parentMediator->getParentLevel()), {0, 0});
            evolutionTower->loadIcon();
            upgradeIcon = (evolutionTower->getIcon());
            upgradeIcon =
                Aligner::align(upgradeIcon, HorizontalAlignment::Center,
                               VerticalAlignment::Middle);
            isCapped = false;
        }
    } else if (parentMediator) {
        price = upgrades->getNextUpgradeDetail(upgradeID)->cost;
        const UpgradeType* upgradeType = upgrades->getUpgradeType(upgradeID);
        if (upgradeType && ResourceManager::getInstance().getTexture(
                               upgradeType->getIconPath()))
            upgradeIcon = sf::Sprite(*ResourceManager::getInstance().getTexture(
                upgradeType->getIconPath()));
        upgradeIcon = Aligner::align(upgradeIcon);
    } else {
        Logger::error("Parent radial menu is not set for UpgradeButton");
        return;
    }
    updatePriceTag();
    updateSpritePosition();
}

UpgradeButton& UpgradeButton::setParentMediator(RadialUpgradeMenu* radialMenu) {
    parentMediator = radialMenu;
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
        if (contains(windowPosition)) {
            graphicState.updatePressState(true);
            return true;
        }
    } else if (event == UserEvent::Release && button == Mouse::Left) {
        if (graphicState.isPressed()) {
            graphicState.updatePressState(false);
            if (contains(windowPosition) && canUpgrade) {
                parentMediator->notify("upgrade", this, upgradeID);
                if (upgrades->isTotalUpgradeLimitReached()) {
                    parentMediator->notify("show_upgrade_preview", this,
                                           nullptr);
                } else
                    if (upgrades->getNextUpgradeDetail(upgradeID) == nullptr &&
                        upgrades->canEvolve(upgradeID))
                        parentMediator->notify(
                            "show_evolution", this,
                            upgrades->getEvolveTo(upgradeID));
                    else
                    parentMediator->notify(
                        "show_upgrade_preview", this,
                        upgrades->getNextUpgradeDetail(upgradeID));
                return true;
            }

            if (contains(windowPosition) && evolutionTower) {
                std::string evolutionID = upgrades->getEvolveTo(upgradeID);
                evolutionTower = nullptr;

                parentMediator->notify("evolution", *this, evolutionID);
                Cursor::getInstance().setHoverText("");
                return true;
            }
            return false;
        }
    } else if (event == UserEvent::Move) {
        if (!graphicState.isHovered() && contains(windowPosition)) {
            graphicState.updateHoverState(true);
            if (upgrades->getEvolveTo(upgradeID) != "") {
                Cursor::getInstance().setHoverText("Can evolve");
            } else {
                Cursor::getInstance().setHoverText("Cannot evolve");
            }
            if (parentMediator) {
                if (upgrades->isTotalUpgradeLimitReached()) {
                    if (upgrades->canEvolve(upgradeID))
                        parentMediator->notify(
                            "show_evolution", *this,
                            upgrades->getEvolveTo(upgradeID));
                    else
                        parentMediator->notify("show_upgrade_preview", *this,
                                               nullptr);
                } else {
                    if (upgrades->getNextUpgradeDetail(upgradeID) == nullptr &&
                        upgrades->canEvolve(upgradeID))
                        parentMediator->notify(
                            "show_evolution", *this,
                            upgrades->getEvolveTo(upgradeID));
                    else
                        parentMediator->notify(
                            "show_upgrade_preview", *this,
                            upgrades->getNextUpgradeDetail(upgradeID));
                }
            }
        } else if (graphicState.isHovered() && !contains(windowPosition)) {
            graphicState.updateHoverState(false);
            if (parentMediator) {
                parentMediator->notify("hide_preview", *this);
            }
            Cursor::getInstance().setHoverText("");
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
    : CircularButton(other),
      radius(other.radius),
      parentMediator(other.parentMediator),
      upgradeID(other.upgradeID),
      canUpgrade(other.canUpgrade),
      price(other.price),
      buttonShape(other.buttonShape),
      upgradeIcon(other.upgradeIcon),
      tagDisplay(other.tagDisplay) {
    // Deep copy for priceTag (sf::RenderTexture)
    // SFML RenderTexture cannot be copied directly, so we copy the texture
    // if available
    updatePriceTag();
}

UpgradeButton& UpgradeButton::setUpgradeManager(UpgradeManager& target) {
    upgrades = &target;
    return *this;
}

UpgradeButton& UpgradeButton::setUpgradeID(int id) {
    this->upgradeID = id;
    return *this;
}

void UpgradeButton::updateSpritePosition() {
    upgradeIcon = Aligner::align(upgradeIcon, HorizontalAlignment::Center,
                                 VerticalAlignment::Middle);
    upgradeIcon.setPosition(buttonShape.getPosition());
}

UpgradeButton& UpgradeButton::setCanUpgrade(bool val) {
    canUpgrade = val;
    return *this;
}

UpgradeButton& UpgradeButton::setIsCapped(bool val) {
    isCapped = val;
    return *this;
}

UpgradeButton& UpgradeButton::setStyle(std::string configFile) {
    nlohmann::json styleConfig = JSONLoader::getInstance().getStyle(configFile);
    graphicState.loadStyle(styleConfig);
    buttonShape.setFillColor(graphicState.getFillColor());
    buttonShape.setOutlineColor(graphicState.getBorderColor());
    buttonShape.setOutlineThickness(graphicState.getStyle().getBorderWidth());
    return *this;
}