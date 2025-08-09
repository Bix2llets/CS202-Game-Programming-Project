#include "Gameplay/TowerInfoPanel.hpp"

#include <format>

#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Utility/aligner.hpp"
TowerInfoPanel& TowerInfoPanel::getInstance() {
    static TowerInfoPanel instance;
    return instance;
}

TowerInfoPanel::TowerInfoPanel()
    : referencingTower(nullptr),
      range(*ResourceManager::getInstance().getFont("pixel")),
      damage(*ResourceManager::getInstance().getFont("pixel")),
      fireRate(*ResourceManager::getInstance().getFont("pixel")),
      maxTargets(*ResourceManager::getInstance().getFont("pixel")),
      rangeIcon{*ResourceManager::getInstance().getTexture("range_icon")},
      damageIcon{*ResourceManager::getInstance().getTexture("power_icon")},
      fireRateIcon{*ResourceManager::getInstance().getTexture("speed_icon")},
      maxTargetsIcon{*ResourceManager::getInstance().getTexture("sell_icon")},
      towerName(*ResourceManager::getInstance().getFont("pixel")) {
    rangeIcon = Aligner::align(rangeIcon, HorizontalAlignment::Center,
                               VerticalAlignment::Middle);
    damageIcon = Aligner::align(damageIcon, HorizontalAlignment::Center,
                                VerticalAlignment::Middle);
    fireRateIcon = Aligner::align(fireRateIcon, HorizontalAlignment::Center,
                                  VerticalAlignment::Middle);
    maxTargetsIcon = Aligner::align(maxTargetsIcon, HorizontalAlignment::Center,
                                    VerticalAlignment::Middle);
    range.setCharacterSize(25);
    damage.setCharacterSize(25);
    fireRate.setCharacterSize(25);
    maxTargets.setCharacterSize(25);
    range.setFillColor(sf::Color::Black);
    damage.setFillColor(sf::Color::Black);
    fireRate.setFillColor(sf::Color::Black);
    maxTargets.setFillColor(sf::Color::Black);
    rangeIcon.setScale({0.5f, 0.5f});
    damageIcon.setScale({0.5f, 0.5f});
    fireRateIcon.setScale({0.5f, 0.5f});
    maxTargetsIcon.setScale({0.5f, 0.5f});

    background.setPosition({GameConstants::MENU_X, 0});
    background.setSize(
        {GameConstants::DEFAULT_WINDOW_WIDTH - GameConstants::MENU_X,
         GameConstants::DEFAULT_WINDOW_HEIGHT});

    background.setFillColor(sf::Color(93, 153, 189, 255));
    background.setOutlineColor(sf::Color(0x9F9491FF));
    background.setOutlineThickness(4);

    damageIcon.setPosition(
        {background.getPosition().x + 20, background.getPosition().y + 120});

    rangeIcon.setPosition(damageIcon.getPosition() +
                          sf::Vector2f{0, (damageIcon.getGlobalBounds().size.y +
                                           rangeIcon.getGlobalBounds().size.y) /
                                                  2.f +
                                              10});
    fireRateIcon.setPosition(
        rangeIcon.getPosition() +
        sf::Vector2f{0, (rangeIcon.getGlobalBounds().size.y +
                         fireRateIcon.getGlobalBounds().size.y) /
                                2.f +
                            10});
    maxTargetsIcon.setPosition(
        fireRateIcon.getPosition() +
        sf::Vector2f{0, (fireRateIcon.getGlobalBounds().size.y +
                         maxTargetsIcon.getGlobalBounds().size.y) /
                                2.f +
                            10});
    range.setPosition(rangeIcon.getGlobalBounds().position +
                      sf::Vector2f{rangeIcon.getGlobalBounds().size.x + 10,
                                   rangeIcon.getGlobalBounds().size.y / 2.f});
    damage.setPosition(damageIcon.getGlobalBounds().position +
                       sf::Vector2f{damageIcon.getGlobalBounds().size.x + 10,
                                    damageIcon.getGlobalBounds().size.y / 2.f});
    fireRate.setPosition(
        fireRateIcon.getGlobalBounds().position +
        sf::Vector2f{fireRateIcon.getGlobalBounds().size.x + 10,
                     fireRateIcon.getGlobalBounds().size.y / 2.f});
    maxTargets.setPosition(
        maxTargetsIcon.getGlobalBounds().position +
        sf::Vector2f{maxTargetsIcon.getGlobalBounds().size.x + 10,
                     maxTargetsIcon.getGlobalBounds().size.y / 2.f});
    towerName.setCharacterSize(30);
    towerName.setFillColor(sf::Color::Black);
}

void TowerInfoPanel::render(sf::RenderStates state) {
    if (!referencingTower) return;
    sf::RenderWindow& window = Window::getInstance().getRenderWindow();
    window.draw(background, state);
    window.draw(damageIcon, state);
    window.draw(rangeIcon, state);
    window.draw(fireRateIcon, state);
    window.draw(maxTargetsIcon, state);
    window.draw(damage, state);
    window.draw(range, state);
    window.draw(fireRate, state);
    window.draw(maxTargets, state);

    sf::Sprite towerSprite = referencingTower->getIcon();
    towerSprite = Aligner::align(towerSprite, HorizontalAlignment::Center,
                                 VerticalAlignment::Middle);

    towerSprite.setPosition(
        {background.getPosition().x + background.getSize().x / 2.f,
         background.getPosition().y + 60});
    towerSprite.setScale({2.f, 2.f});
    towerName.setString(referencingTower->getName());
    towerName = Aligner::align(towerName);
    towerName.setPosition(
        towerSprite.getPosition() -
        sf::Vector2f{0, towerSprite.getGlobalBounds().size.y / 2.f - 20});
    window.draw(towerSprite, state);
    window.draw(towerName, state);

    // Render the tower information panel
    // This is a placeholder for actual rendering logic
    // You can use sf::Text, sf::Sprite, etc. to display tower information
}

void TowerInfoPanel::setFocus(Tower* tower) {
    referencingTower = tower;

    update();
    // Additional logic to update the panel with tower information
}

void TowerInfoPanel::deFocus() {
    referencingTower = nullptr;
    // Additional logic to clear the panel
}

void TowerInfoPanel::update() {
    if (!referencingTower) return;

    float rangeValue = referencingTower->getStat(TowerStat::RANGE);
    float damageValue = referencingTower->getStat(TowerStat::DAMAGE);
    float fireRateValue = referencingTower->getStat(TowerStat::FIRE_RATE);
    float maxTargetsValue = referencingTower->getStat(TowerStat::MAX_TARGETS);
    range.setString(std::format("{:.2f}", rangeValue));
    damage.setString(std::format("{:.2f}", damageValue));
    fireRate.setString(std::format("{:.2f}", fireRateValue));
    maxTargets.setString(std::format("{:.2f}", maxTargetsValue));

    range = Aligner::align(range, HorizontalAlignment::Left,
                           VerticalAlignment::Middle);
    damage = Aligner::align(damage, HorizontalAlignment::Left,
                            VerticalAlignment::Middle);
    fireRate = Aligner::align(fireRate, HorizontalAlignment::Left,
                              VerticalAlignment::Middle);
    maxTargets = Aligner::align(maxTargets, HorizontalAlignment::Left,
                                VerticalAlignment::Middle);
    // Update the panel with the current state of the tower
    // This could include updating text, sprites, etc.
}
