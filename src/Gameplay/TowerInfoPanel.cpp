#include "Gameplay/TowerInfoPanel.hpp"

#include <format>

#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "Entity/Factory/TowerFactory.hpp"
#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Entity/Tower/Tower.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Scene/Level.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include "Utility/TargetSelectorCycle.hpp"
// TowerInfoPanel& TowerInfoPanel::getInstance() {
//     static TowerInfoPanel instance;
//     return instance;
// }

TowerInfoPanel::TowerInfoPanel()
    : referencingTower(nullptr),
      range(*ResourceManager::getInstance().getFont("text")),
      damage(*ResourceManager::getInstance().getFont("text")),
      fireRate(*ResourceManager::getInstance().getFont("text")),
      maxTargets(*ResourceManager::getInstance().getFont("text")),
      rangeIcon{*ResourceManager::getInstance().getTexture("range_icon")},
      damageIcon{*ResourceManager::getInstance().getTexture("power_icon")},
      fireRateIcon{*ResourceManager::getInstance().getTexture("speed_icon")},
      maxTargetsIcon{*ResourceManager::getInstance().getTexture("sell_icon")},
      towerName(*ResourceManager::getInstance().getFont("pixel")),
      upgradeTitle(*ResourceManager::getInstance().getFont("pixel")),
      towerSprite(GameConstants::BLANK_TEXTURE),
      evolutionSprite(GameConstants::BLANK_TEXTURE),
      evolutionTitle(*ResourceManager::getInstance().getFont("pixel")),
      evolutionName(*ResourceManager::getInstance().getFont("text")),
      enemySelectionStrat{*ResourceManager::getInstance().getFont("text")} {
    rangeIcon = Aligner::align(rangeIcon, HorizontalAlignment::Center,
                               VerticalAlignment::Middle);
    damageIcon = Aligner::align(damageIcon, HorizontalAlignment::Center,
                                VerticalAlignment::Middle);
    fireRateIcon = Aligner::align(fireRateIcon, HorizontalAlignment::Center,
                                  VerticalAlignment::Middle);
    maxTargetsIcon = Aligner::align(maxTargetsIcon, HorizontalAlignment::Center,
                                    VerticalAlignment::Middle);
    range.setCharacterSize(16);
    damage.setCharacterSize(16);
    fireRate.setCharacterSize(16);
    maxTargets.setCharacterSize(16);
    range.setFillColor(sf::Color::Black);
    damage.setFillColor(sf::Color::Black);
    fireRate.setFillColor(sf::Color::Black);
    maxTargets.setFillColor(sf::Color::Black);
    rangeIcon.setScale({0.5f, 0.5f});
    damageIcon.setScale({0.5f, 0.5f});
    fireRateIcon.setScale({0.5f, 0.5f});
    maxTargetsIcon.setScale({0.5f, 0.5f});

    background.setPosition({GameConstants::MENU_X, 145});
    background.setSize(
        {GameConstants::DEFAULT_WINDOW_WIDTH - GameConstants::MENU_X,
         GameConstants::DEFAULT_WINDOW_HEIGHT});

    background.setTexture(
        ResourceManager::getInstance().getTexture("tower_info_background"));
    background.setFillColor(sf::Color::White);
    // background.setOutlineColor(sf::Color(0x9F9491FF));
    // background.setOutlineThickness(4);

    damageIcon.setPosition(
        {background.getPosition().x + 20, background.getPosition().y + 150});

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

    upgradeTitle.setString("Upgrade");
    upgradeTitle.setCharacterSize(30);
    upgradeTitle.setFillColor(sf::Color::Black);
    upgradeTitle = Aligner::align(upgradeTitle);
    upgradeTitle.setPosition(
        {background.getPosition().x + background.getSize().x / 2.f,
         background.getPosition().y + 320});

    previewClosingTimer.setRemainingTime(0.25f)
        .setTimeInterval(0.25f)
        .setTimerMode(TimerMode::Single);

    prevModeButton =
        RectangularButtonBuilder(*this)
            .reset()
            .setSize({40, 40})
            .setPosition({background.getPosition().x + 0,
                          background.getPosition().y + 255})
            .setCallback([this](RectangularButton* button) {
                if (!referencingTower) return;
                                this->notify("prev_mode", this, nullptr);
            })
            .loadJson("borderless_background_basic")
            .setBackground(ResourceManager::getInstance().getTexture("prev"))
            .build();

    nextModeButton =
        RectangularButtonBuilder(*this)
            .reset()
            .setSize({40, 40})
            .setPosition({background.getPosition().x + 160,
                          background.getPosition().y + 255})
            .setCallback([this](RectangularButton* button) {
                if (!referencingTower) return;
                this->notify("next_mode", this, nullptr);
            })
            .loadJson("borderless_background_basic")
            .setBackground(ResourceManager::getInstance().getTexture("next"))
            .build();

    enemySelectionStrat.setPosition(
        {(nextModeButton->getPosition().x + prevModeButton->getSize().x + prevModeButton->getPosition().x) /
             2.f,
         nextModeButton->getPosition().y + nextModeButton->getSize().y / 2.f});
    enemySelectionStrat.setCharacterSize(16);
    enemySelectionStrat.setFillColor(sf::Color::Black);

    subscribe("next_mode", [this](std::any sender, std::any data) {
        if (!referencingTower) return;
        referencingTower->getCombatBehavior()->setTargetSelector(SelectorCycle::nextSelector(
            referencingTower->getCombatBehavior()->getTargetSelector()));
        update();
    });
    subscribe("prev_mode", [this](std::any sender, std::any data) {
        if (!referencingTower) return;
        referencingTower->getCombatBehavior()->setTargetSelector(SelectorCycle::prevSelector(
            referencingTower->getCombatBehavior()->getTargetSelector()));
        update();
    });
}

void TowerInfoPanel::render(sf::RenderStates state) const {
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

    window.draw(towerSprite, state);
    window.draw(towerName, state);
    window.draw(upgradeTitle);

    window.draw(*nextModeButton);
    window.draw(*prevModeButton);
    window.draw(enemySelectionStrat);

    for (const auto& text : upgradeContents) {
        window.draw(text, state);
    }

    if (evolutionPreviewTower) {
        window.draw(evolutionSprite, state);
        window.draw(evolutionTitle, state);
        window.draw(evolutionName, state);
    }
    // Render the tower information panel
    // This is a placeholder for actual rendering logic
    // You can use sf::Text, sf::Sprite, etc. to display tower information
}

void TowerInfoPanel::setFocus(Tower* tower) {
    referencingTower = tower;

    const UpgradeManager* upgradeManager =
        referencingTower->getUpgradeManager();
    if (upgradeManager && !upgradeManager->isTotalUpgradeLimitReached()) {
    }
    towerSprite = referencingTower->getIcon();
    towerSprite = Aligner::align(towerSprite, HorizontalAlignment::Center,
                                 VerticalAlignment::Middle);

    towerSprite.setPosition(
        {background.getPosition().x + background.getSize().x / 2.f,
         background.getPosition().y + 100});
    towerSprite.setScale({2.f, 2.f});
    towerName.setString(referencingTower->getName());
    towerName = Aligner::align(towerName);
    towerName.setPosition(
        towerSprite.getPosition() -
        sf::Vector2f{0, towerSprite.getGlobalBounds().size.y / 2.f});

    update();
}

void TowerInfoPanel::deFocus() {
    referencingTower = nullptr;
    // Additional logic to clear the panel
}

void TowerInfoPanel::update() {
    if (!referencingTower) return;
    this->previewClosingTimer.update();
    float rangeValue = referencingTower->getStat(TowerStat::RANGE);
    float damageValue = referencingTower->getStat(TowerStat::DAMAGE);
    float fireRateValue = referencingTower->getStat(TowerStat::FIRE_RATE);
    float maxTargetsValue = referencingTower->getStat(TowerStat::MAX_TARGETS);
    range.setString(std::format("{:.2f}", rangeValue));
    damage.setString(std::format("{:.2f}", damageValue));
    fireRate.setString(std::format("{:.2f}", fireRateValue));
    maxTargets.setString(std::format("{:.2f}", maxTargetsValue));

    enemySelectionStrat.setString(
        referencingTower->getCombatBehavior()->getTargetSelector()->getName());
    enemySelectionStrat =
        Aligner::align(enemySelectionStrat, HorizontalAlignment::Center,
                       VerticalAlignment::Middle);
    range = Aligner::align(range, HorizontalAlignment::Left,
                           VerticalAlignment::Middle);
    damage = Aligner::align(damage, HorizontalAlignment::Left,
                            VerticalAlignment::Middle);
    fireRate = Aligner::align(fireRate, HorizontalAlignment::Left,
                              VerticalAlignment::Middle);
    maxTargets = Aligner::align(maxTargets, HorizontalAlignment::Left,
                                VerticalAlignment::Middle);

    prevModeButton->update();
    nextModeButton->update();
}

void TowerInfoPanel::displayUpgrade(const UpgradeDetails* detail) {
    if (!detail) {
        upgradeContents.clear();
        currentUpgradeDetail = nullptr;
        return;
    }
    currentUpgradeDetail = detail;
    upgradeContents.clear();

    const auto& upgrades = currentUpgradeDetail->bonusStats.getAllStats();
    upgradeContents.resize(
        upgrades.size(),
        sf::Text(*ResourceManager::getInstance().getFont("text")));
    int index = 0;
    for (const auto& upgrade : upgrades) {
        std::string upgradeName = upgrade.first;
        if (upgradeName.substr(0, 7) == "effect_") {
            upgradeName = upgradeName.substr(7);
        }
        for (int i = 0; i < upgradeName.size(); i++) {
            if (upgradeName[i] == '_') {
                upgradeName[i] = ' ';
                continue;
            }
            if (i == 0) {
                upgradeName[i] = toupper(upgradeName[i]);
                continue;
            }
            if (upgradeName[i - 1] == ' ') {
                upgradeName[i] = toupper(upgradeName[i]);
                continue;
            }
        }
        float currentStat = referencingTower->getStat(upgrade.first);
        float nextStat = currentStat + upgrade.second;
        upgradeContents[index].setString(std::format(
            "{}:\n{:.2f} -> {:.2f}", upgradeName, currentStat, nextStat));
        upgradeContents[index].setCharacterSize(16);
        upgradeContents[index].setLineSpacing(1.5f);
        if (upgrade.second > 0) {
            upgradeContents[index].setFillColor(sf::Color::Green);
        } else {
            upgradeContents[index].setFillColor(sf::Color::Red);
        }

        upgradeContents[index] =
            Aligner::align(upgradeContents[index], HorizontalAlignment::Left,
                           VerticalAlignment::Middle);
        upgradeContents[index].setPosition(
            {background.getPosition().x + 20,
             upgradeTitle.getPosition().y + 50 + index * 70});
        index++;
    }
    this->previewClosingTimer.reset();
}

void TowerInfoPanel::clearDisplayUpgrade() {
    if (!previewClosingTimer.isAvailable()) return;
    currentUpgradeDetail = nullptr;
    upgradeContents.clear();
    evolutionTitle.setString("");
    evolutionSprite = sf::Sprite(GameConstants::BLANK_TEXTURE);
    evolutionPreviewTower = nullptr;
}

bool TowerInfoPanel::onMouseEvent(Mouse button, UserEvent event,
                                  const sf::Vector2f& worldPosition,
                                  const sf::Vector2f& windowPosition) {
    if (!referencingTower) return false;
    if (prevModeButton->onMouseEvent(button, event, worldPosition,
                                     windowPosition))
        return true;
    if (nextModeButton->onMouseEvent(button, event, worldPosition,
                                     windowPosition))
        return true;

    return false;
}

bool TowerInfoPanel::onScrollEvent(float delta,
                                   const sf::Vector2f& worldPosition,
                                   const sf::Vector2f& windowPosition) {
    return false;
}

void TowerInfoPanel::displayEvolution(const std::string& evolveTo,
                                      Level* level) {
    if (!referencingTower) return;
    upgradeContents.clear();
    const EntityStat* stats = referencingTower->getStats();
    for (auto [id, value] : stats->getAllStats()) {
        if (!(id == TowerStat::RANGE || id == TowerStat::DAMAGE ||
              id == TowerStat::FIRE_RATE || id == TowerStat::MAX_TARGETS)) {
            continue;
        }
        std::string statName = id;
        for (int i = 0; i < statName.size(); i++) {
            if (statName[i] == '_') {
                statName[i] = ' ';
                continue;
            }
            if (i == 0) {
                statName[i] = toupper(statName[i]);
                continue;
            }
            if (statName[i - 1] == ' ') {
                statName[i] = toupper(statName[i]);
                continue;
            }
        }
        upgradeContents.push_back(
            sf::Text(*ResourceManager::getInstance().getFont("text")));
        upgradeContents.back().setString(
            std::format("{}: \n{:.2f} -> {:.2f}", statName,
                        referencingTower->getStat(id), value));
        upgradeContents.back().setCharacterSize(16);
        upgradeContents.back().setLineSpacing(1.5f);
        if (value > referencingTower->getStat(id)) {
            upgradeContents.back().setFillColor(sf::Color::Green);
        } else if (value < referencingTower->getStat(id)) {
            upgradeContents.back().setFillColor(sf::Color::Red);
        } else
            upgradeContents.back().setFillColor(sf::Color::Black);

        upgradeContents.back() =
            Aligner::align(upgradeContents.back(), HorizontalAlignment::Left,
                           VerticalAlignment::Middle);

        upgradeContents.back().setPosition(
            {background.getPosition().x + 20,
             upgradeTitle.getPosition().y + 50 +
                 (upgradeContents.size() - 1) * 60});
    }
    evolutionPreviewTower =
        TowerFactory::createFromConfigFile(evolveTo, *level);
    evolutionSprite = evolutionPreviewTower->getIcon();
    evolutionSprite = Scaler::scaleSprite(evolutionSprite,
                                          towerSprite.getGlobalBounds().size);
    evolutionSprite =

        Aligner::align(evolutionSprite, HorizontalAlignment::Center,
                       VerticalAlignment::Middle);

    evolutionTitle.setString("EVOLUTION");
    Aligner::align(evolutionTitle);

    evolutionTitle.setCharacterSize(30);
    evolutionTitle.setFillColor(sf::Color::Black);
    evolutionTitle.setPosition(
        {background.getPosition().x + background.getSize().x / 2.f,
         upgradeContents.back().getPosition().y +
             upgradeContents.back().getGlobalBounds().size.y / 2.f + 50});

    evolutionSprite.setPosition(
        evolutionTitle.getPosition() +
        sf::Vector2f{0, evolutionTitle.getGlobalBounds().size.y / 2.f} +
        sf::Vector2f{0, 50});

    evolutionName.setString(evolutionPreviewTower->getName());
    evolutionName.setCharacterSize(16);
    evolutionName.setFillColor(sf::Color::Black);
    evolutionName = Aligner::align(evolutionName, HorizontalAlignment::Center,
                                   VerticalAlignment::Middle);
    evolutionName.setPosition(
        evolutionSprite.getPosition() +
        sf::Vector2f{0, evolutionSprite.getGlobalBounds().size.y / 2.f} +
        sf::Vector2f{0, evolutionName.getGlobalBounds().size.y / 2.f} +
        sf::Vector2f{0, -10});
}