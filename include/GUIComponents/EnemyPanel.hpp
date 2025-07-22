/**
 * @file EnemyPanel.hpp
 * @brief Declares the EnemyPanel GUI component.
 *
 * This panel is implemented using the singleton pattern to ensure only one
 * instance exists. It is responsible for displaying basic statistics about the
 * currently selected enemy, such as health, attack power, and other relevant
 * attributes.
 */
#pragma once

#include <SFML/Graphics.hpp>
class Enemy;
class EnemyPanel : public sf::Drawable {
    // * Singleton related
   private:
    EnemyPanel();
    EnemyPanel(const EnemyPanel& other) = delete;
    EnemyPanel(const EnemyPanel&& other) = delete;
    EnemyPanel& operator=(const EnemyPanel& rhs) = delete;
    EnemyPanel& operator=(EnemyPanel&& rhs) = delete;

   public:
    static EnemyPanel& getInstance();
    // * For enemy reference
   private:
    const Enemy* displayingEnemy;

   public:
    void clearEnemy();
    void clearEnemyIfReferencing(const Enemy& enemy);
    void setEnemy(const Enemy& enemy);

    inline const Enemy* const getDisplayingEnemy() { return displayingEnemy; }
    // * Render info
   private:
    sf::Text health;
    const sf::Sprite* enemySprite;
    sf::Sprite healthIcon;

    float previousHealth;

   public:
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
};