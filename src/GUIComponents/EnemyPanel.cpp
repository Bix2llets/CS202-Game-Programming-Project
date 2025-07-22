#include "GUIComponents/EnemyPanel.hpp"

#include <format>
#include <string>

#include "Core/ResourceManager.hpp"
#include "Entity/Enemy/Enemy.hpp"
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
      speedIcon(*ResourceManager::getInstance().getTexture("movement_speed")) {
    calibratePosition();
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
            std::format("{} / {}", displayingEnemy->health.getHealth(),
                        displayingEnemy->health.getMaxHealth());

        health.setString(content);
    }
    float currentSpeed = displayingEnemy->getSpeed();
    Logger::debug(std::format("{:.2f}", currentSpeed));
    if (currentSpeed != previousSpeed) {
        previousSpeed = currentSpeed;
        isChanged = true;
        speed.setString(std::format("{:.2f}", currentSpeed));
    }
    if (isChanged) calibratePosition();
}

void EnemyPanel::setEnemy(const Enemy& enemy) {
    displayingEnemy = &enemy;
    previousHealth = enemy.getHealth();
    previousSpeed = enemy.getSpeed();
    std::string content =
        std::format("{} / {}", displayingEnemy->health.getHealth(),
                    displayingEnemy->health.getMaxHealth());

    health.setString(content);

    speed.setString(std::format("{:.2f}", displayingEnemy->getSpeed()));

    calibratePosition();
}

void EnemyPanel::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    if (displayingEnemy == nullptr) return;
    sf::Sprite enemySprite = displayingEnemy->sprite;
    enemySprite.setPosition(popupCoordinate);
    enemySprite.setRotation(sf::degrees(-90.f));
    target.draw(health);
    target.draw(healthIcon);
    target.draw(speed);
    target.draw(speedIcon);
    target.draw(enemySprite);
    auto drawOriginMarker = [&target, &state](sf::Transformable object) {
        sf::CircleShape positionMarker(2.f);
        positionMarker.setOrigin({2.f, 2.f});
        positionMarker.setPosition(object.getPosition());
        positionMarker.setFillColor(sf::Color::White);
        target.draw(positionMarker);
    };
    // drawOriginMarker(health);
    // drawOriginMarker(healthIcon);
    // drawOriginMarker(speed);
    // drawOriginMarker(speedIcon);

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
    // Logger::debug(std::format("{} {}", ringSprite.getPosition().x,
    //                           ringSprite.getPosition().y));

    ringSprite.setOrigin(ringSprite.getLocalBounds().size / 2.f);
    ringSprite.setPosition(displayingEnemy->position);
    // Draw the ring texture to the target
    target.draw(ringSprite, state);
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

void EnemyPanel::calibratePosition() {
    fixOrigin(healthIcon);
    healthIcon.setPosition(popupCoordinate + sf::Vector2f{20.f, 0.f});
    healthIcon.setScale({0.25f, 0.25f});

    fixOrigin(health);
    health.setPosition(healthIcon.getPosition() + sf::Vector2f{9.f, 0.f} +
                       sf::Vector2f{healthIcon.getGlobalBounds().size.x, 0});

    fixOrigin(speedIcon);
    speedIcon.setPosition(health.getPosition() + sf::Vector2f{25.f, 0.f} +
                          sf::Vector2f{health.getGlobalBounds().size.x, 0});
    speedIcon.setScale({0.25f, 0.25f});

    fixOrigin(speed);
    speed.setPosition(speedIcon.getPosition() +
                      sf::Vector2f{speedIcon.getGlobalBounds().size.x, 0} +
                      sf::Vector2f{10.f, 0.f});
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