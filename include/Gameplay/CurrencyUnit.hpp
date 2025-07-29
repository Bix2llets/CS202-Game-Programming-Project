#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "Base/Constants.hpp"
struct CurrencyUnit {
    int value = 0;
    std::string description = "Placeholder";
    sf::Sprite icon{GameConstants::BLANK_TEXTURE};
};