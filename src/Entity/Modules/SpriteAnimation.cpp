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

    nlohmann::json viewport = jsonFile["viewport"];

    sf::Vector2i numberOfFrame = {
        static_cast<int>(viewport["width"].get<int>() / width),
        static_cast<int>(viewport["height"].get<int>() / height)};

    for (int j = 0; j < numberOfFrame.y; j++)
        for (int i = 0; i < numberOfFrame.x; i++)
            sprites.push_back(sf::Sprite(
                *texture, sf::IntRect{{viewport["x"].get<int>() + i * width,
                                       viewport["y"].get<int>() + j * height},
                                      {width, height}}));
    sf::Vector2f origin = sf::Vector2f{jsonFile["centerX"].get<float>(),
                                       jsonFile["centerY"].get<float>()};

    for (sf::Sprite& sprite : sprites) sprite.setOrigin(origin);
    std::string animationType = jsonFile["type"];
    if (animationType == "circular") type = AnimationType::Circular;
    if (animationType == "linear") type = AnimationType::Linear;
    animationTimer = std::make_unique<Timer>();
    animationTimer->setTimeInterval(jsonFile["animationInterval"])
        .setTimerMode(TimerMode::Continuous)
        .setTimerDirection(TimerDirection::Backward)
        .setRemainingTime(jsonFile["animationInterval"]);
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

void SpriteAnimation::resume() { animationTimer->resume(); }

void SpriteAnimation::pause() { animationTimer->pause(); }
