#ifndef RESOURCE_TOWER_HPP
#define RESOURCE_TOWER_HPP

#include "Entity/Tower/Tower.hpp"

/**
 * @brief Resource Tower base class for towers that generate resources
 * 
 * These towers generate resources over time rather than attacking enemies.
 * Examples include gold mines, energy generators, etc.
 */
class ResourceTower : public Tower {
protected:
    int resourceAmount;     // Amount of resources generated per cycle
    float generationRate;   // How often resources are generated (seconds)
    float lastGenerateTime; // Time of last resource generation

public:
    /**
     * @brief Construct a new Resource Tower object
     * 
     * @param pos Position to place the tower
     * @param amount Amount of resources generated per cycle
     * @param rate How often resources are generated (seconds)
     */
    ResourceTower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
                 int amount = 10, float rate = 5.0f);

    /**
     * @brief Destructor
     */
    ~ResourceTower() override = default;

    /**
     * @brief Update the resource tower's behavior
     * 
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime) override;

    // Getters and setters
    int getResourceAmount() const { return resourceAmount; }
    void setResourceAmount(int amount) { resourceAmount = amount; }
    
    float getGenerationRate() const { return generationRate; }
    void setGenerationRate(float rate) { generationRate = rate; }
};

#endif // RESOURCE_TOWER_HPP
