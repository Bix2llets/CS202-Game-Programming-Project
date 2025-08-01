#include "Utility/CollisionChecker.hpp"

bool CollisionChecker::isSegmentCrossed(sf::Vector2f p11, sf::Vector2f p12,
                                        sf::Vector2f p21, sf::Vector2f p22) {
    auto orientation = [](sf::Vector2f p1, sf::Vector2f p2,
                          sf::Vector2f p3) -> int {
        float value =
            (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
        if (value == 0) return value;
        return value / abs(value);
    };
    return (orientation(p11, p12, p21) != orientation(p11, p12, p22)) &&
           (orientation(p21, p22, p11) != orientation(p21, p22, p12));
}

bool CollisionChecker::isQuadilateralCrossed(sf::Vector2f shape1[4],
                                             sf::Vector2f shape2[4]) {
    for (int i1 = 0; i1 < 4; i1++)
        for (int j1 = i1 + 1; j1 < 4; j1++)
            for (int i2 = 0; i2 < 4; i2++)
                for (int j2 = i2 + 1; j2 < 4; j2++)
                    if (isSegmentCrossed(shape1[i1], shape1[j1], shape2[i2],
                                         shape2[j2]))
                        return true;
    return false;
}