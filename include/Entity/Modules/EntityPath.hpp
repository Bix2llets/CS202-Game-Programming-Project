
#pragma once

#include <vector>

#include "Gameplay/Waypoint.hpp"
class EntityPath {
   private:
    sf::Vector2f position;
    const std::vector<Waypoint>* waypoints;
    float speed;
    float distanceFromStart;
    float terrainMultiplier;
    float effectMultiplier;
    int waypointIndex;

   public:
    const std::vector<Waypoint>* getWaypoints() const;
    sf::Vector2f getPosition() const;
    float getDistanceFromStart() const;
    float getOriginalSpeed() const;
    float getActualSpeed() const;

    void setDistanceFromStart(float distance);
    void setSpeed(float speed);
    void setEffectSpeedModifier(float multiplier);
    void setWaypoints(const std::vector<Waypoint>* newWaypoints);
    void update();

    inline bool isFinished() const {
        return waypoints->size() == 0 || waypointIndex == waypoints->size() - 1;
    }

    EntityPath() : terrainMultiplier{1.f} {}

    sf::Angle angleByVertical();
};