#include "Scene/Scene.hpp"

#include "Core/ResourceManager.hpp"
const sf::Font* const Scene::getFont(std::string fontName) {
    return resourceManager.getFont(fontName);
}