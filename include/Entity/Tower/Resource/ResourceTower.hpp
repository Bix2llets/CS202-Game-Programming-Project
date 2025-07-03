
#pragma once

#include "Entity/Tower/Tower.hpp"

/**
 * @brief Resource Tower base class for towers that generate resources
 * 
 * These towers generate resources over time rather than attacking enemies.
 * Examples include gold mines, energy generators, etc.
 */
class ResourceTower : public Tower {
protected:
    int totalResource;
    int generationRate;     // Amount of resources generated per cycle
    float generationInterval;   // Time interval between each generation
    float remainingTime; // Time since last generation

public:
    /**
     * @brief Construct a new Resource Tower object
     * 
     * @param pos Position to place the tower
     * @param amount Amount of resources generated per cycle
     * @param rate Time interval between each generation
     */
    ResourceTower(const sf::Vector2f& pos = sf::Vector2f(0, 0),
                 int amount = 10, float rate = 5.0f);

    /**
     * @brief Destructor
     */
    ~ResourceTower() override = default;

    /**
     * @brief Update the resource tower's behavior
     */
    void update() override;

    // Getters and setters

    inline float getGenerationRate() const { return generationRate; }
    inline float getRemainingTime() const {return remainingTime; }
    inline float getGenerationInterval() const {return generationInterval;}
    inline float getTotalResource() const {return totalResource; }


    inline void setGenerationRate(float rate) { generationRate = rate; }
    inline void setGenerationInterval(float interval) { generationInterval = interval; }
    inline void setRemainingTime(float time) { remainingTime = time; }
    inline void setTotalResource(int total) { totalResource = total; }
};

 