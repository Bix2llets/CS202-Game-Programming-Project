#include "Entity/Modules/SpriteAnimation.hpp"

SpriteAnimation::SpriteAnimation() 
    : type(AnimationType::Linear), currentFrame(0) {
    // Default constructor - initialize members
}

void SpriteAnimation::loadJson(ResourceManager& resManager,
                               const nlohmann::json& jsonFile) {
    const sf::Texture* texture = resManager.getTexture(jsonFile["textureID"]);
    int width = static_cast<int>(jsonFile["width"]);
    int height = static_cast<int>(jsonFile["height"]);
    sf::Vector2i numberOfFrame = {
        static_cast<int>(texture->getSize().x / width),
        static_cast<int>(texture->getSize().y / height)};

    for (int j = 0; j < numberOfFrame.y; j++)
        for (int i = 0; i < numberOfFrame.x; i++)
            sprites.push_back(sf::Sprite(
                *texture, sf::IntRect{{i * width, j * height}, {width, height}}));

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
