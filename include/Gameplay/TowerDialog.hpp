#pragma once

#include <SFML/Graphics.hpp>

enum class DialogPosition {
    TopLeft,
    TopRight,
};

class TowerDialog {

    void draw(sf::RenderTarget &target, sf::RenderStates state) const;

};