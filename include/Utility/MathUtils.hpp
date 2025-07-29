#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

/**
 * @file MathUtils.hpp
 * @brief Utility class for mathematical operations and calculations.
 *
 * Contains functions for common mathematical operations used throughout
 * the game, particularly for angle calculations and vector operations.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>


/**
 * @class MathUtils
 * @brief Static utility class providing mathematical helper functions.
 *
 * All functions are static. This class provides common mathematical operations
 * like angle calculations, distance measurements, and vector operations.
 */
class MathUtils {
public:
    /**
     * @brief Math constant for PI.
     */
    static constexpr float PI = 3.14159265358979323846f;

    /**
     * @brief Calculate the angle from one position to another.
     * @param from The starting position.
     * @param to The target position.
     * @return sf::Angle The angle from 'from' to 'to' in radians.
     */
    static sf::Angle calculateAngleTo(const sf::Vector2f& from, const sf::Vector2f& to);

    /**
     * @brief Calculate the direction vector from one position to another.
     * @param from The starting position.
     * @param to The target position.
     * @return sf::Vector2f The direction vector (not normalized).
     */
    static sf::Vector2f calculateDirection(const sf::Vector2f& from, const sf::Vector2f& to);

    /**
     * @brief Calculate the Euclidean distance between two positions.
     * @param from The first position.
     * @param to The second position.
     * @return float The Euclidean distance between the positions.
     */
    static float calculateDistance(const sf::Vector2f& from, const sf::Vector2f& to);

    /**
     * @brief Calculate the squared distance between two positions.
     *        Useful for distance comparisons without the expensive sqrt operation.
     * @param from The first position.
     * @param to The second position.
     * @return float The squared distance between the positions.
     */
    static float calculateDistanceSquared(const sf::Vector2f& from, const sf::Vector2f& to);

    /**
     * @brief Normalize a vector to unit length.
     * @param vector The vector to normalize.
     * @return sf::Vector2f The normalized vector, or zero vector if input has zero length.
     */
    static sf::Vector2f normalize(const sf::Vector2f& vector);

    /**
     * @brief Check if a point is within a circular range of another point.
     * @param center The center position.
     * @param point The point to check.
     * @param range The circular range radius.
     * @return bool True if the point is within range, false otherwise.
     */
    static bool isWithinRange(const sf::Vector2f& center, const sf::Vector2f& point, float range);

    /**
     * @brief Convert degrees to radians.
     * @param degrees The angle in degrees.
     * @return float The angle in radians.
     */
    static float degreesToRadians(float degrees);

    /**
     * @brief Convert radians to degrees.
     * @param radians The angle in radians.
     * @return float The angle in degrees.
     */
    static float radiansToDegrees(float radians);

private:
    /**
     * @brief Private constructor to prevent instantiation.
     */
    MathUtils() = default;
};

#endif // MATHUTILS_HPP
