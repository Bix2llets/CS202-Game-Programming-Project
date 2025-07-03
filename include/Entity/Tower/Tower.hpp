
#pragma once

#include "Entity/Entity.hpp"
#include <memory>

/**
 * @brief Base class for all tower entities
 * 
 * This is the base class for different types of towers in the game.
 * Specific functionality is implemented in derived classes.
 */
class Tower : public Entity {
protected:
    int cost;               // Cost to build the tower
    int level;              // Current upgrade level
    int maxLevel;           // Maximum upgrade level
    std::optional<sf::Sprite> baseSprite;  // Fixed base sprite that doesn't rotate

public:
    /**
     * @brief Construct a new Tower object
     * 
     * @param pos Position to place the tower
     * @param buildCost Cost to build the tower
     */
    Tower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
          int buildCost = 100);

    /**
     * @brief Destructor
     */
    ~Tower() override = default;

    /**
     * @brief Update the tower's behavior
     */
    virtual void update();

    /**
     * @brief Render the tower
     * 
     * @param target Render target to draw on
     */
    void draw(sf::RenderTarget &target, sf::RenderStates state) const override = 0;
    
    /**
     * @brief Update both base sprite and upper layer sprite positions
     * 
     * @param pos New position
     */
    void setPosition(const sf::Vector2f& pos);

    /**
     * @brief Upgrade the tower to the next level
     * 
     * @return bool True if upgrade successful, false if at max level
     */
    virtual bool upgrade();

    // Getters and setters
    int getCost() const { return cost; }
    void setCost(int newCost) { cost = newCost; }
    
    int getLevel() const { return level; }
    void setLevel(int newLevel) { level = newLevel; }
    /**
     * @brief Create and set a base sprite from a texture
     * 
     * @param texture The texture to use for the base sprite
     */
    void loadBaseTexture(const sf::Texture& texture);
};


