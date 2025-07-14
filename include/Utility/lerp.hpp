#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
class ColorMixer {
    private:
    struct Oklab {
        float L, a, b;
    };
    static Oklab rgbToOklab(sf::Color color);

    static sf::Color oklabToRgb(const Oklab &lab);

   public:
    static sf::Color perceptualLerp(const sf::Color &start,
                                    const sf::Color &end, float t);

    static sf::Color linearLerp(const sf::Color &start, const sf::Color &end, float t);
};