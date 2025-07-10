#include "Entity/Modules/SpriteAnimation.hpp"

SpriteAnimation::SpriteAnimation() {
    // * Assuming the sprite is a horiozontal strip
}

void SpriteAnimation::loadJson(ResourceManager& resManager,
                               const nlohmann::json& jsonFile) {
    const sf::Texture* texture = resManager.getTexture(jsonFile["textureID"]);
    sf::Vector2i numberOfFrame = {
        static_cast<int>(texture->getSize().x / jsonFile["sizeX"]),
        static_cast<int>(texture->getSize().y / jsonFile["sizeY"])};

    for (int j = 0; j < numberOfFrame.y; j++)
        for (int i = 0; i < numberOfFrame.x; i++)
            sprites.push_back(sf::Sprite(
                *texture,
                sf::IntRect{{i * jsonFile["sizeX"], j * jsonFile["sizeY"]},
                            {jsonFile["sizeX"], jsonFile["sizeY"]}}));

    if (jsonFile["type"] == "circular") type = AnimationType::Circular;
    if (jsonFile["type"] == "linear") type = AnimationType::Linear;

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

sf::Sprite SpriteAnimation::getCurrentSprite() {
    return sprites[currentFrame];
}

void SpriteAnimation::restart() {
    animationTimer->reset();
    currentFrame = 0;
}

