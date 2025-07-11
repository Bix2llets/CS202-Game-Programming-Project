#include "Scene/Scene.hpp"

#include "Core/ResourceManager.hpp"
const sf::Font* const Scene::getFont(std::string fontName) {
    return resourceManager.getFont(fontName);
}

void Scene::loadTexture(const std::string& path, const std::string& ID) {
    resourceManager.loadTexture(path, ID);
}

const sf::Texture* const Scene::getTexture(const std::string& ID) {
    return resourceManager.getTexture(ID);
}