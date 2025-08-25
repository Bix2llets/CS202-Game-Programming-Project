#include "GUIComponents/EnemyPanel.hpp"

#include <format>
#include <string>

#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
EnemyPanel& EnemyPanel::getInstance() {
    static EnemyPanel instance;
    return instance;
}

EnemyPanel::EnemyPanel()
    : displayingEnemy{nullptr},
      health{*ResourceManager::getInstance().getFont("pixel")},
      speed(*ResourceManager::getInstance().getFont("pixel")),
      healthIcon(*ResourceManager::getInstance().getTexture("heart")),
      speedIcon(*ResourceManager::getInstance().getTexture("movement_speed")),
      name{*ResourceManager::getInstance().getFont("pixel")},
      burnIcon{*ResourceManager::getInstance().getTexture("burn_icon")},
      napalmIcon{*ResourceManager::getInstance().getTexture("napalm_icon")},
      regenIcon{*ResourceManager::getInstance().getTexture("regen_icon")},
      vulnerableIcon{
          *ResourceManager::getInstance().getTexture("vulnerable_icon")},
      resistanceIcon{
          *ResourceManager::getInstance().getTexture("resistance_icon")},
      fireResistanceIcon{
          *ResourceManager::getInstance().getTexture("fire_resistance_icon")},
      energizedIcon{
          *ResourceManager::getInstance().getTexture("energized_icon")},
      slowIcon{*ResourceManager::getInstance().getTexture("slow_icon")} {
    enemyBackdrop.setPosition(popupPosition);
    enemyBackdrop.setRadius(40.f);
    enemyBackdrop.setFillColor(sf::Color{189, 193, 198, 255});
    enemyBackdrop.setOutlineColor(sf::Color{14, 70, 140, 255});
    enemyBackdrop.setOutlineThickness(3.f);
    enemyBackdrop.setOrigin(
        {enemyBackdrop.getRadius(), enemyBackdrop.getRadius()});

    Scaler::scaleSprite(healthIcon, {16.f, 16.f});
    Scaler::scaleSprite(speedIcon, {16.f, 16.f});
    // healthIcon.setScale({0.25f, 0.25f});
    // speedIcon.setScale({0.25f, 0.25f});

    health.setFillColor(sf::Color::Black);
    speed.setFillColor(sf::Color::Black);
    name.setFillColor(sf::Color::Black);
    health.setOutlineColor(sf::Color::White);
    speed.setOutlineColor(sf::Color::White);
    name.setOutlineColor(sf::Color::White);
    health.setOutlineThickness(0.5f);
    speed.setOutlineThickness(0.5f);
    name.setOutlineThickness(0.5f);

    statBackdrop.setSize(sf::Vector2f{500.f, 50.f});
    statBackdrop.setPosition(popupPosition);
    statBackdrop.setFillColor(sf::Color{189, 193, 198, 255});
    statBackdrop.setOutlineColor(sf::Color{14, 70, 140, 255});
    statBackdrop.setOutlineThickness(3.f);

    healthBar.setSize(statBackdrop.getSize());
    healthBar.setPosition(popupPosition);
    healthBar.setFillColor(sf::Color(17, 130, 0, 255));

    fixOrigin(healthBar);
    fixOrigin(statBackdrop);
    fixOrigin(healthIcon);
    fixOrigin(speedIcon);
    name.setPosition(popupPosition + sf::Vector2f(60.f, 0.f));

    healthIcon.setPosition(popupPosition + sf::Vector2f{200.f, 0.f} +
                           sf::Vector2f{name.getLocalBounds().position.x +
                                            name.getLocalBounds().size.x,
                                        0} +
                           sf::Vector2f{25.f, 0.f});

    // fixOrigin(health);
    health.setPosition(healthIcon.getPosition() + sf::Vector2f{9.f, 0.f} +
                       sf::Vector2f{healthIcon.getGlobalBounds().size.x, 0});

    // fixOrigin(speedIcon);
    speedIcon.setPosition(popupPosition + sf::Vector2f{360.f, 0.f} +
                          sf::Vector2f{25.f, 0.f} +
                          sf::Vector2f{health.getGlobalBounds().size.x, 0});

    // fixOrigin(speed);
    speed.setPosition(speedIcon.getPosition() +
                      sf::Vector2f{speedIcon.getGlobalBounds().size.x, 0} +
                      sf::Vector2f{10.f, 0.f});
    auto configureIcon = [](sf::Sprite& icon) {
        Scaler::scaleSprite(icon, {12.f, 12.f});
        Aligner::align(icon, HorizontalAlignment::Center,
                       VerticalAlignment::Middle);
    };

    configureIcon(burnIcon);
    configureIcon(napalmIcon);
    configureIcon(regenIcon);
    configureIcon(vulnerableIcon);
    configureIcon(resistanceIcon);
    configureIcon(fireResistanceIcon);
    configureIcon(energizedIcon);
    configureIcon(slowIcon);

    burnIcon.setPosition(
        statBackdrop.getPosition() +
        sf::Vector2f{statBackdrop.getSize().x - 20.f - 8.f, statBackdrop.getSize().y - 16.f - 16.f});
    napalmIcon.setPosition(burnIcon.getPosition() +
                           sf::Vector2f{-6.f - 5.f, 0.f});
    regenIcon.setPosition(napalmIcon.getPosition() +
                          sf::Vector2f{-6.f - 5.f, 0.f});
    vulnerableIcon.setPosition(regenIcon.getPosition() +
                               sf::Vector2f{-6.f - 5.f, 0.f});
    resistanceIcon.setPosition(vulnerableIcon.getPosition() +
                               sf::Vector2f{-6.f - 5.f, 0.f});
    fireResistanceIcon.setPosition(resistanceIcon.getPosition() +
                                   sf::Vector2f{-6.f - 5.f, 0.f});
    energizedIcon.setPosition(fireResistanceIcon.getPosition() +
                              sf::Vector2f{-6.f - 5.f, 0.f});
    slowIcon.setPosition(energizedIcon.getPosition() +
                         sf::Vector2f{-6.f - 5.f, 0.f});
}

void EnemyPanel::update() {
    if (displayingEnemy == nullptr) return;
    float enemyHealth = displayingEnemy->health.getHealth();
    bool isChanged = false;
    if (enemyHealth != previousHealth) {
        // * Update the text only when health changes
        previousHealth = enemyHealth;
        isChanged = true;
        std::string content =
            std::format("{:.0f}", displayingEnemy->health.getHealth());

        health.setString(content);
        healthBar.setSize({statBackdrop.getSize().x * enemyHealth /
                               displayingEnemy->health.getMaxHealth(),
                           statBackdrop.getSize().y});
    }
    float currentSpeed = displayingEnemy->getSpeed();
    // Logger::debug(std::format("{:.2f}", currentSpeed));
    if (currentSpeed != previousSpeed) {
        previousSpeed = currentSpeed;
        isChanged = true;
        speed.setString(std::format("{:.0f}", previousSpeed));
    }
    if (isChanged) calibrate();
}

void EnemyPanel::setEnemy(const Enemy& enemy) {
    displayingEnemy = &enemy;
    previousHealth = enemy.getHealth();
    previousSpeed = enemy.getSpeed();
    name.setString(displayingEnemy->name);
    Logger::debug(displayingEnemy->name);
    std::string content =
        std::format("{:.0f}", displayingEnemy->health.getHealth());

    health.setString(content);
    healthBar.setSize(
        {statBackdrop.getSize().x * displayingEnemy->health.getHealth() /
             displayingEnemy->health.getMaxHealth(),
         statBackdrop.getSize().y});

    speed.setString(std::format("{:.0f}", displayingEnemy->getSpeed()));

    fixOrigin(health);
    fixOrigin(speed);
    fixOrigin(name);

    calibrate();
}

void EnemyPanel::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    if (displayingEnemy == nullptr) return;
    sf::Sprite enemySprite = displayingEnemy->sprite;
    enemySprite.setPosition(popupPosition);
    enemySprite.setRotation(sf::degrees(-90.f));

    // * Background
    target.draw(statBackdrop);
    target.draw(healthBar);
    target.draw(enemyBackdrop);

    // * Foreground
    target.draw(health);
    target.draw(healthIcon);
    target.draw(speed);
    target.draw(speedIcon);
    target.draw(enemySprite);
    target.draw(name);
    auto drawOriginMarker = [&target, &state](sf::Transformable object) {
        sf::CircleShape positionMarker(2.f);
        positionMarker.setOrigin({2.f, 2.f});
        positionMarker.setPosition(object.getPosition());
        positionMarker.setFillColor(sf::Color::White);
        Window::getInstance().toggleGUIMode();
        target.draw(positionMarker);
    };
    drawOriginMarker(health);
    drawOriginMarker(healthIcon);
    drawOriginMarker(speed);
    drawOriginMarker(speedIcon);

    sf::RenderTexture ringTexture(
        static_cast<sf::Vector2u>(
            displayingEnemy->sprite.getLocalBounds().size) +
        sf::Vector2u{40, 40});
    sf::CircleShape innerRing(20.f, 6);
    sf::CircleShape outerRing(25.f, 6);
    ringTexture.clear(sf::Color::Transparent);
    innerRing.setRotation(displayingEnemy->sprite.getRotation());
    outerRing.setRotation(displayingEnemy->sprite.getRotation());
    // Draw outer ring
    outerRing.setFillColor(sf::Color(116, 122, 118, 255));
    outerRing.setOutlineThickness(0.f);
    outerRing.setOrigin({outerRing.getRadius(), outerRing.getRadius()});
    outerRing.setPosition(static_cast<sf::Vector2f>(ringTexture.getSize()) /
                          2.f);
    ringTexture.draw(outerRing, sf::RenderStates(sf::BlendNone));

    // Draw inner ring
    innerRing.setFillColor(sf::Color::Transparent);
    innerRing.setOutlineThickness(0.f);
    innerRing.setOrigin({innerRing.getRadius(), innerRing.getRadius()});
    innerRing.setPosition(static_cast<sf::Vector2f>(ringTexture.getSize()) /
                          2.f);
    ringTexture.draw(innerRing, sf::RenderStates(sf::BlendNone));

    ringTexture.display();
    sf::Sprite ringSprite(ringTexture.getTexture());

    ringSprite.setOrigin(ringSprite.getLocalBounds().size / 2.f);
    Window::getInstance().toggleUserMode();
    ringSprite.setPosition(displayingEnemy->position);
    // Draw the ring texture to the target
    target.draw(ringSprite, state);
    Window::getInstance().toggleGUIMode();

    if (displayingEnemy->getEffects().hasEffectType(EffectType::Burn)) {
        target.draw(burnIcon, state);
    }

    if (displayingEnemy->getEffects().hasEffectType(EffectType::NapalmBurn)) {
        target.draw(napalmIcon, state);
    }

    if (displayingEnemy->getEffects().hasEffectType(EffectType::Regeneration)) {
        target.draw(regenIcon, state);
    }

    if (displayingEnemy->getEffects().hasEffectType(EffectType::Vulnerable)) {
        target.draw(vulnerableIcon, state);
    }

    if (displayingEnemy->getEffects().hasEffectType(EffectType::Resistance)) {
        target.draw(resistanceIcon, state);
    }

    if (displayingEnemy->getEffects().hasEffectType(EffectType::FireResistance)) {
        target.draw(fireResistanceIcon, state);
    }

    if (displayingEnemy->getEffects().hasEffectType(EffectType::Energized)) {
        target.draw(energizedIcon, state);
    }

    if (displayingEnemy->getEffects().hasEffectType(EffectType::Slow)) {
        target.draw(slowIcon, state);
    }
}

void EnemyPanel::clearEnemy() {
    displayingEnemy = nullptr;
    health.setString("");
    previousHealth = 0.0f;
}

void EnemyPanel::clearEnemyIfReferencing(const Enemy& enemy) {
    if (displayingEnemy == &enemy) {
        clearEnemy();
    }
}

void EnemyPanel::calibrate() {
    // fixOrigin(healthIcon);
    // name.setPosition(popupPosition + sf::Vector2f(60.f, 0.f));

    // healthIcon.setPosition(name.getPosition() +
    //                        sf::Vector2f{name.getLocalBounds().position.x +
    //                                         name.getLocalBounds().size.x,
    //                                     0} +
    //                        sf::Vector2f{25.f, 0.f});

    // // fixOrigin(health);
    // health.setPosition(healthIcon.getPosition() + sf::Vector2f{9.f, 0.f} +
    //                    sf::Vector2f{healthIcon.getGlobalBounds().size.x, 0});

    // // fixOrigin(speedIcon);
    // speedIcon.setPosition(health.getPosition() + sf::Vector2f{25.f, 0.f} +
    //                       sf::Vector2f{health.getGlobalBounds().size.x, 0});

    // // fixOrigin(speed);
    // speed.setPosition(speedIcon.getPosition() +
    //                   sf::Vector2f{speedIcon.getGlobalBounds().size.x, 0} +
    //                   sf::Vector2f{10.f, 0.f});

    // fixOrigin(statBackdrop);
}

void EnemyPanel::fixOrigin(sf::Sprite& target) {
    target.setOrigin({0, target.getLocalBounds().position.y +
                             target.getLocalBounds().size.y / 2.f});
}
void EnemyPanel::fixOrigin(sf::Shape& target) {
    target.setOrigin({0, target.getLocalBounds().position.y +
                             target.getLocalBounds().size.y / 2.f});
}
void EnemyPanel::fixOrigin(sf::Text& target) {
    target.setOrigin({0, target.getLocalBounds().position.y +
                             target.getLocalBounds().size.y / 2.f});
}