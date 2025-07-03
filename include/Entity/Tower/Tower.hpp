#ifndef TOWER_HPP
#define TOWER_HPP

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
     * 
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime) override;

    /**
     * @brief Render the tower
     * 
     * @param target Render target to draw on
     */
    void render(sf::RenderTarget& target) override;
    
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
    
    int getMaxLevel() const { return maxLevel; }
    void setMaxLevel(int newMax) { maxLevel = newMax; }
    
    /**
     * @brief Set the base sprite for this tower (the non-rotating part)
     * 
     * @param newBaseSprite The sprite to use for the base
     */
    void setBaseSprite(const sf::Sprite& newBaseSprite);

    /**
     * @brief Create and set a base sprite from a texture
     * 
     * @param texture The texture to use for the base sprite
     */
    void setBaseSpriteTexture(const sf::Texture& texture);

    /**
     * @brief Check if the tower has a base sprite
     * 
     * @return bool True if base sprite is set, false otherwise
     */
    bool hasBaseSprite() const { return baseSprite.has_value(); }

    /**
     * @brief Get the base sprite (if available)
     * 
     * @return const sf::Sprite* Pointer to base sprite or nullptr if not set
     */
    const sf::Sprite* getBaseSprite() const { 
        return baseSprite.has_value() ? &baseSprite.value() : nullptr; 
    }
};

#endif // TOWER_HPP
