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
      health{*ResourceManager::getInstance().getFont("pixel")} {
    health.setPosition({100, 800});
}

void EnemyPanel::update() {
    if (displayingEnemy == nullptr) return;
    float enemyHealth = displayingEnemy->health.getHealth();
    if (enemyHealth != previousHealth) {
        // * Update the text only when health changes
        std::string content =
            std::format("{} / {}", displayingEnemy->health.getHealth(),
                        displayingEnemy->health.getMaxHealth());

        health.setString(content);
        previousHealth = enemyHealth;
    }
}

void EnemyPanel::setEnemy(const Enemy& enemy) {
    displayingEnemy = &enemy;
    enemySprite = &enemy.sprite;
    previousHealth = enemy.getHealth();
    std::string content =
        std::format("{} / {}", displayingEnemy->health.getHealth(),
                    displayingEnemy->health.getMaxHealth());

    health.setString(content);
    health.setOrigin({0, health.getLocalBounds().size.y / 2});
}

void EnemyPanel::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    if (displayingEnemy == nullptr) return;
    target.draw(health);

    sf::RenderTexture ringTexture(
        static_cast<sf::Vector2u>(
            displayingEnemy->sprite.getLocalBounds().size) +
        sf::Vector2u{40, 40});
    sf::CircleShape innerRing(20.f);
    sf::CircleShape outerRing(25.f);
    ringTexture.clear(sf::Color::Transparent);

    // Draw outer ring
    outerRing.setFillColor(sf::Color(116, 122, 118, 255));
    outerRing.setOutlineThickness(0.f);
    outerRing.setOrigin({outerRing.getRadius(), outerRing.getRadius()});
    outerRing.setPosition(static_cast<sf::Vector2f>(ringTexture.getSize()) / 2.f);
    ringTexture.draw(outerRing, sf::RenderStates(sf::BlendNone));

    // Draw inner ring
    innerRing.setFillColor(sf::Color::Transparent);
    innerRing.setOutlineThickness(0.f);
    innerRing.setOrigin({innerRing.getRadius(), innerRing.getRadius()});
    innerRing.setPosition(static_cast<sf::Vector2f>(ringTexture.getSize()) / 2.f);
    ringTexture.draw(innerRing, sf::RenderStates(sf::BlendNone));

    ringTexture.display();
    sf::Sprite ringSprite(ringTexture.getTexture());
    Logger::debug(std::format("{} {}", ringSprite.getPosition().x,
                              ringSprite.getPosition().y));

    ringSprite.setOrigin(ringSprite.getLocalBounds().size / 2.f);
    ringSprite.setPosition(displayingEnemy->position);
    // Draw the ring texture to the target
    target.draw(ringSprite, state);
}

void EnemyPanel::clearEnemy() {
    displayingEnemy = nullptr;
    enemySprite = nullptr;
    health.setString("");
    previousHealth = 0.0f;
}

void EnemyPanel::clearEnemyIfReferencing(const Enemy& enemy) {
    if (displayingEnemy == &enemy) {
        clearEnemy();
    }
}