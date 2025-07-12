
#include "Entity/Modules/EntityPath.hpp"

#include <SFML/System/Vector2.hpp>
#include <algorithm>

#include "Base/Constants.hpp"

sf::Vector2f EntityPath::getPosition() const { return position; }

float EntityPath::getDistanceFromStart() const { return distanceFromStart; }

void EntityPath::setDistanceFromStart(float distance) {
    distanceFromStart = std::max(0.f, distance);

    // Find the segment where the distance falls
    float accumulated = 0.f;
    for (size_t i = 0; i < waypoints->size() - 1; ++i) {
        float segmentLength =
            ((*waypoints)[i + 1].position - (*waypoints)[i].position).length();
        if (accumulated + segmentLength >= distanceFromStart) {
            waypointIndex = i;
            float t = (distanceFromStart - accumulated) / segmentLength;
            position =
                (*waypoints)[i].position +
                t * ((*waypoints)[i + 1].position - (*waypoints)[i].position);
            return;
        }
        accumulated += segmentLength;
    }
    // If distance exceeds path, set to last waypoint
    waypointIndex = waypoints->size() - 2;
    position = (*waypoints)[waypoints->size() - 1].position;
}

float EntityPath::getOriginalSpeed() const { return speed; }
float EntityPath::getActualSpeed() const { return speed * multiplier; }

void EntityPath::setSpeed(float s) { speed = s; }

void EntityPath::update() {
    distanceFromStart += speed * GameConstants::TICK_INTERVAL * multiplier;

    float remainingTravelDistance =
        speed * GameConstants::TICK_INTERVAL * multiplier;
    while (remainingTravelDistance > 0 &&
           waypointIndex + 1 < waypoints->size()) {
        sf::Vector2f unitVector = (waypoints->at(waypointIndex + 1).position -
                                   waypoints->at(waypointIndex).position)
                                      .normalized();
        sf::Vector2f supposedNextPosition =
            position + remainingTravelDistance * unitVector;

        sf::Vector2f segmentEndToSupposed =
            ((*waypoints)[waypointIndex + 1].position - supposedNextPosition);
        sf::Vector2f segmentStartToEnd =
            ((*waypoints)[waypointIndex].position - supposedNextPosition);
        if ((segmentEndToSupposed).dot(segmentStartToEnd) >= 0) {
            waypointIndex++;
            remainingTravelDistance -=
                ((*waypoints)[waypointIndex].position - position).length();
            position = (*waypoints)[waypointIndex].position;
            multiplier *= (*waypoints)[waypointIndex].speedMultiplier;
        } else {
            position = supposedNextPosition;
            remainingTravelDistance = 0;
        }
    }
    if (waypointIndex == waypoints->size() - 1)
        position = waypoints->back().position;
}

const std::vector<Waypoint>* EntityPath::getWaypoints() const {
    return waypoints;
}

void EntityPath::setWaypoints(const std::vector<Waypoint>* newWaypoints) {
    waypoints = newWaypoints;
    waypointIndex = 0;
    distanceFromStart = 0.f;
    if (waypoints && !waypoints->empty()) {
        position = waypoints->front().position;
        multiplier = waypoints->front().speedMultiplier;
    }
}

sf::Angle EntityPath::angleByVertical() {
    if (waypointIndex == waypoints->size() - 1) return sf::Angle(sf::radians(0));
    sf::Vector2f displacement = ((*waypoints)[waypointIndex + 1].position -
                                 (*waypoints)[waypointIndex].position)
                                    .normalized();
    sf::Vector2f vertical = sf::Vector2f{0, -1}.normalized();
    return displacement.angle();
}