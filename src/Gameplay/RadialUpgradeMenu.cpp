#include "Gameplay/RadialUpgradeMenu.hpp"

#include <cmath>

#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Scene/Level.hpp"
#include "Utility/Aligner.hpp"
#include "Utility/Logger.hpp"
#include "Utility/Scaler.hpp"
RadialUpgradeMenu::RadialUpgradeMenu(Level& parentLevel)
    : parentLevel{parentLevel} {
    ring.setFillColor(sf::Color(0, 0, 0, 0));
    ring.setOutlineThickness(4);
    ring.setOutlineColor(sf::Color::Black);
    ring = Aligner::align(ring, HorizontalAlignment::Center,
                          VerticalAlignment::Middle);
    sellBtn.setParentRadialMenu(this)
        .setRadius(35)
        .setDisplaySprite(
            sf::Sprite(*ResourceManager::getInstance().getTexture("sell_icon")))
        .setStyle("background_basic");
    subscribe("upgrade", [this, &parentLevel](std::any sender, std::any data) {
        try {
            int upgradeID = std::any_cast<int>(data);
            parentLevel.notify(
                "subtract_currency", *this,
                upgradeManager->getNextUpgradeDetail(upgradeID)->cost);

            upgradeManager->upgrade(upgradeID);

        } catch (std::bad_any_cast& e) {
            Logger::error(std::format(
                "RadialUpgradeMenu: Bad any cast in upgrade event handler: {}",
                e.what()));
        }
    });

    subscribe("sell", [this, &parentLevel](std::any sender, std::any data) {
        try {
            parentLevel.notify("sell_tower", refTower);
        } catch (std::bad_any_cast& e) {
            Logger::error(std::format(
                "RadialUpgradeMenu: Bad any cast in sell event handler: {}",
                e.what()));
        }
    });
    subscribe("show_upgrade_preview", [this](std::any sender, std::any data) {
        this->parentLevel.notify("show_upgrade_preview", *this, data);
    });
    subscribe("hide_upgrade_preview", [this](std::any sender, std::any data) {
        this->parentLevel.notify("hide_upgrade_preview", *this);
    });
}

void RadialUpgradeMenu::setFocus(Tower* tower) {
    refTower = tower;
    Window::getInstance().toggleGUIMode();
    position = static_cast<sf::Vector2f>(Window::getInstance().getRenderWindow().mapCoordsToPixel(tower->getPosition()));
    upgradeManager = tower->getUpgradeManager();
    displaying = true;
    nlohmann::json towerInfo =
        JSONLoader::getInstance().getTower(tower->getId());
    float width = static_cast<float>(towerInfo["texture"]["width"]);
    float height = static_cast<float>(towerInfo["texture"]["height"]);

    float newRadius = std::sqrt(width * width + height * height + 3);
    upgradeButtons.clear();
    upgradeButtons.resize(upgradeManager->getAllUpgradeTypes().size());

    ring.setPosition(position);
    ring.setRadius(128);
    ring = Aligner::align(ring, HorizontalAlignment::Center,
                          VerticalAlignment::Middle);

    sf::Angle upgradeDisplayInterval = sf::degrees(360 * 4.f / 6.f);
    sf::Angle startingAngle =
        sf::degrees(0.f) + sf::degrees(60.f) + sf::degrees(90.f);

    for (int i = 0; i < upgradeButtons.size(); i++) {
        sf::Angle angle;
        if (upgradeButtons.size() == 1) {
            angle = sf::degrees(-90.f);
        } else {
            angle = startingAngle +
                    upgradeDisplayInterval / (upgradeButtons.size() - 1) * i;
        }
        sf::Vector2f displacement = {newRadius + 2, 0};
        displacement = displacement.rotatedBy(angle);

        upgradeButtons[i]
            .setParentRadialMenu(this)
            .setRadius(36)
            .setPosition(position + displacement)
            .setStyle(std::string("background_basic"))
            .setUpgradeManager(*upgradeManager)
            .setUpgradeID(i + 1);

        upgradeButtons[i].setUpgradeManager(*upgradeManager);
        upgradeButtons[i].setUpgradeID(i + 1);
        upgradeButtons[i].refreshInfo();
        upgradeButtons[i].resetAnimation();
    }
    sellBtn.setPosition(
        sf::Vector2f(newRadius + 2, 0).rotatedBy(sf::degrees(90)) + position);

    sellBtn.resetAnimation();
    update();
}

void RadialUpgradeMenu::removeFocus() {
    position = {-1000, -1000};
    upgradeManager = nullptr;
    displaying = false;
    refTower = nullptr;
}

const UpgradeDetails* RadialUpgradeMenu::getUpgradeDetail(int id) {
    if (upgradeManager) {
        return upgradeManager->getNextUpgradeDetail(id);
    } else {
        Logger::error("RadialUpgradeMenu: UpgradeManager is not set");
        return {};
    }
}

bool RadialUpgradeMenu::onMouseEvent(Mouse button, UserEvent event,
                                     const sf::Vector2f& worldPosition,
                                     const sf::Vector2f& windowPosition) {
    if (!displaying) return false;

    for (auto& upgradeButton : upgradeButtons) {
        if (upgradeButton.onMouseEvent(button, event, worldPosition,
                                       windowPosition)) {
            return true;
        }
    }

    if (sellBtn.onMouseEvent(button, event, worldPosition, windowPosition)) {
        return true;
    }

    return false;
}

bool RadialUpgradeMenu::onScrollEvent(float delta,
                                      const sf::Vector2f& worldPosition,
                                      const sf::Vector2f& windowPosition) {
    return false;
}

void RadialUpgradeMenu::render(sf::RenderStates state) const {
    if (!displaying) return;

    sf::RenderTarget& target = Window::getInstance().getRenderWindow();
    Window::getInstance().toggleGUIMode();
    target.draw(ring, state);

    for (const auto& upgradeButton : upgradeButtons) {
        target.draw(upgradeButton, state);
    }

    target.draw(sellBtn, state);

    sf::CircleShape rangeIndicator;
    rangeIndicator.setRadius(refTower->getStat(TowerStat::RANGE));
    rangeIndicator.setOrigin(
        {rangeIndicator.getRadius(), rangeIndicator.getRadius()});
    rangeIndicator.setPosition(refTower->getPosition());
    rangeIndicator.setFillColor(sf::Color(0, 0, 0, 100));
    Window::getInstance().toggleUserMode();
    target.draw(rangeIndicator);
}

void RadialUpgradeMenu::update() {
    if (upgradeManager)

        if (upgradeManager->isTotalUpgradeLimitReached()) {
            for (auto& button : upgradeButtons) {
                button.setIsCapped(true);
            }
        }

    for (int i = 0; i < upgradeManager->getAllUpgradeTypes().size(); i++) {
        if (upgradeManager->canUpgrade(i + 1, parentLevel.getBudget()))
            upgradeButtons[i].setCanUpgrade(true);
        else
            upgradeButtons[i].setCanUpgrade(false);
    }
    updatePositions();
    for (auto& button : upgradeButtons) button.update();
    sellBtn.update();
}

void RadialUpgradeMenu::updatePositions() {
    Window::getInstance().toggleUserMode();
    position = static_cast<sf::Vector2f>(
        Window::getInstance().getRenderWindow().mapCoordsToPixel(
            refTower->getPosition()));

    if (position.x + ring.getRadius() + sellBtn.getRadius() >
        GameConstants::MENU_X) {
        position.x =
            GameConstants::MENU_X - ring.getRadius() - sellBtn.getRadius();
    }

    if (position.x - ring.getRadius() - sellBtn.getRadius() < 0) {
        position.x = ring.getRadius() + sellBtn.getRadius();
    }
    if (position.y + ring.getRadius() + sellBtn.getRadius() >
        GameConstants::DEFAULT_WINDOW_HEIGHT) {
        position.y = GameConstants::DEFAULT_WINDOW_HEIGHT - ring.getRadius() -
                     sellBtn.getRadius();
    }

    if (position.y - ring.getRadius() - sellBtn.getRadius() < 0) {
        position.y = ring.getRadius() + sellBtn.getRadius();
    }
    sf::Angle upgradeDisplayInterval = sf::degrees(360 * 4.f / 6.f);
    sf::Angle startingAngle =
        sf::degrees(0.f) + sf::degrees(60.f) + sf::degrees(90.f);
    ring.setPosition(position);
    ring = Aligner::align(ring, HorizontalAlignment::Center,
                          VerticalAlignment::Middle);

    for (int i = 0; i < upgradeButtons.size(); i++) {
        sf::Angle angle;
        if (upgradeButtons.size() == 1) {
            angle = sf::degrees(-90.f);
        } else {
            angle = startingAngle +
                    upgradeDisplayInterval / (upgradeButtons.size() - 1) * i;
        }
        sf::Vector2f displacement = {ring.getRadius() + 2, 0};
        displacement = displacement.rotatedBy(angle);

        upgradeButtons[i].setPosition(position + displacement);
    }

    sellBtn.setPosition(
        position +
        sf::Vector2f(ring.getRadius() + 2, 0).rotatedBy(sf::degrees(90)));
}