#include "Entity/Modules/SpriteAnimation.hpp"

SpriteAnimation::SpriteAnimation() 
    : type(AnimationType::Linear), currentFrame(0) {
    // Default constructor - initialize members
}

void SpriteAnimation::loadJson(ResourceManager& resManager,
                               const nlohmann::json& jsonFile) {
    const sf::Texture* texture = resManager.getTexture(jsonFile["textureID"]);
    int sizeX = static_cast<int>(jsonFile["sizeX"]);
    int sizeY = static_cast<int>(jsonFile["sizeY"]);
    sf::Vector2i numberOfFrame = {
        static_cast<int>(texture->getSize().x / sizeX),
        static_cast<int>(texture->getSize().y / sizeY)};

    for (int j = 0; j < numberOfFrame.y; j++)
        for (int i = 0; i < numberOfFrame.x; i++)
            sprites.push_back(sf::Sprite(
                *texture, sf::IntRect{{i * sizeX, j * sizeY}, {sizeX, sizeY}}));

    std::string animationType = jsonFile["type"];
    if (animationType == "circular") type = AnimationType::Circular;
    if (animationType == "linear") type = AnimationType::Linear;

    animationTimer = std::make_unique<Timer>(jsonFile["animationInterval"],
                                             jsonFile["animationInterval"],
                                             TimerMode::Continuous);
}

void SpriteAnimation::update() {
    if (type == AnimationType::Linear && currentFrame == sprites.size() - 1)
        return;

    animationTimer->update();
    while (animationTimer->isAvailable()) {
        animationTimer->use();
        currentFrame++;
        if (currentFrame == sprites.size()) currentFrame = 0;
    }
}

sf::Sprite SpriteAnimation::getCurrentSprite() { return sprites[currentFrame]; }

void SpriteAnimation::restart() {
    animationTimer->reset();
    currentFrame = 0;
}
