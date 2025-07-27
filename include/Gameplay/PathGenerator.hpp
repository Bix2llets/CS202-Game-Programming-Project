#include <SFML/Graphics.hpp>

#include "Gameplay/Height.hpp"

#pragma once
/**
 * @class PathGenerator
 * @details Specifically engineered for the map. Changes in terrain generation
 * need review and modification of this class
 * Taking a height map whose values ranges from 0 to 7
 */

#include <vector>
class PathGenerator {
   private:
    struct Weight {
        int x;
        int y;
        int distanceFromStart;  // * Deterministic
        int distanceToEnd;      // * Heuristic, use manhattan
    };

    struct WeightCmp {
        bool operator()(Weight a, Weight b) {
            int sumA = a.distanceFromStart + a.distanceToEnd;
            int sumB = b.distanceFromStart + b.distanceToEnd;
            // if (sumA == sumB)
            // if (a.distanceToEnd == b.distanceToEnd)
            //     return a.distanceFromStart > b.distanceFromStart;
            //     return a.distanceToEnd > b.distanceToEnd;
            return sumA > sumB;
        }
    };
    const std::vector<std::vector<Height::Height>> &heightMap;
    std::vector<int> heightCost = {1500, 900, 700, 100, 700, 900, 1200, 1500};
    std::vector<std::vector<int>> transitionCost;

   public:
    PathGenerator(const std::vector<std::vector<Height::Height>> &heightMap);

    std::vector<sf::Vector2f> operator()(sf::Vector2i begin, sf::Vector2i end);
};