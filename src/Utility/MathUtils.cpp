/**
 * @file MathUtils.cpp
 * @brief Implementation of MathUtils utility functions.
 */
#include "Utility/MathUtils.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>


sf::Angle MathUtils::calculateAngleTo(const sf::Vector2f& from, const sf::Vector2f& to) {
    sf::Vector2f direction = to - from;
    float angleRadians = std::atan2(direction.y, direction.x);
    return sf::radians(angleRadians);
}

sf::Vector2f MathUtils::calculateDirection(const sf::Vector2f& from, const sf::Vector2f& to) {
    return to - from;
}

float MathUtils::calculateDistance(const sf::Vector2f& from, const sf::Vector2f& to) {
    sf::Vector2f direction = to - from;
    return std::sqrt(direction.x * direction.x + direction.y * direction.y);
}

float MathUtils::calculateDistanceSquared(const sf::Vector2f& from, const sf::Vector2f& to) {
    sf::Vector2f direction = to - from;
    return direction.x * direction.x + direction.y * direction.y;
}

sf::Vector2f MathUtils::normalize(const sf::Vector2f& vector) {
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (length == 0.0f) {
        return sf::Vector2f(0.0f, 0.0f);
    }
    return sf::Vector2f(vector.x / length, vector.y / length);
}

bool MathUtils::isWithinRange(const sf::Vector2f& center, const sf::Vector2f& point, float range) {
    float distanceSquared = calculateDistanceSquared(center, point);
    return distanceSquared <= (range * range);
}

float MathUtils::degreesToRadians(float degrees) {
    return degrees * (PI / 180.0f);
}

float MathUtils::radiansToDegrees(float radians) {
    return radians * (180.0f / PI);
}
