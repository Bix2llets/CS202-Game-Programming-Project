#pragma once 

#include <SFML/Graphics.hpp>
class CollisionChecker {
    
    public:
    static bool isSegmentCrossed(sf::Vector2f p11, sf::Vector2f p12, sf::Vector2f p21, sf::Vector2f p22);
    static bool isQuadilateralCrossed(sf::Vector2f shape1[4], sf::Vector2f shape2[4]);

};