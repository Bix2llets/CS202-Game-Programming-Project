#include "Entity/Modules/SpriteAnimation.hpp"
#include "Core/ResourceManager.hpp"
#include "Utility/logger.hpp"
#include <stdexcept>

SpriteAnimation::SpriteAnimation()
    : type(AnimationType::Linear), currentFrame(0) {
    // Default constructor - initialize members
}

void SpriteAnimation::loadJson(const nlohmann::json& jsonFile) {
    try {
        // Validate required fields exist
        if (!jsonFile.contains("texture_id")) {
            Logger::error("SpriteAnimation::loadJson: Missing required field 'texture_id'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Missing required field 'texture_id'");
        }
        if (!jsonFile.contains("width")) {
            Logger::error("SpriteAnimation::loadJson: Missing required field 'width'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Missing required field 'width'");
        }
        if (!jsonFile.contains("height")) {
            Logger::error("SpriteAnimation::loadJson: Missing required field 'height'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Missing required field 'height'");
        }
        if (!jsonFile.contains("viewport")) {
            Logger::error("SpriteAnimation::loadJson: Missing required field 'viewport'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Missing required field 'viewport'");
        }
        if (!jsonFile.contains("center_x")) {
            Logger::error("SpriteAnimation::loadJson: Missing required field 'center_x'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Missing required field 'center_x'");
        }
        if (!jsonFile.contains("center_y")) {
            Logger::error("SpriteAnimation::loadJson: Missing required field 'center_y'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Missing required field 'center_y'");
        }
        if (!jsonFile.contains("type")) {
            Logger::error("SpriteAnimation::loadJson: Missing required field 'type'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Missing required field 'type'");
        }
        if (!jsonFile.contains("animation_interval")) {
            Logger::error("SpriteAnimation::loadJson: Missing required field 'animation_interval'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Missing required field 'animation_interval'");
        }

        // Validate field types
        if (!jsonFile["texture_id"].is_string()) {
            Logger::error("SpriteAnimation::loadJson: Field 'texture_id' must be a string");
            throw std::invalid_argument("SpriteAnimation::loadJson: Field 'texture_id' must be a string");
        }
        if (!jsonFile["width"].is_number_integer()) {
            Logger::error("SpriteAnimation::loadJson: Field 'width' must be an integer");
            throw std::invalid_argument("SpriteAnimation::loadJson: Field 'width' must be an integer");
        }
        if (!jsonFile["height"].is_number_integer()) {
            Logger::error("SpriteAnimation::loadJson: Field 'height' must be an integer");
            throw std::invalid_argument("SpriteAnimation::loadJson: Field 'height' must be an integer");
        }
        if (!jsonFile["viewport"].is_object()) {
            Logger::error("SpriteAnimation::loadJson: Field 'viewport' must be an object");
            throw std::invalid_argument("SpriteAnimation::loadJson: Field 'viewport' must be an object");
        }
        if (!jsonFile["center_x"].is_number()) {
            Logger::error("SpriteAnimation::loadJson: Field 'center_x' must be a number");
            throw std::invalid_argument("SpriteAnimation::loadJson: Field 'center_x' must be a number");
        }
        if (!jsonFile["center_y"].is_number()) {
            Logger::error("SpriteAnimation::loadJson: Field 'center_y' must be a number");
            throw std::invalid_argument("SpriteAnimation::loadJson: Field 'center_y' must be a number");
        }
        if (!jsonFile["type"].is_string()) {
            Logger::error("SpriteAnimation::loadJson: Field 'type' must be a string");
            throw std::invalid_argument("SpriteAnimation::loadJson: Field 'type' must be a string");
        }
        if (!jsonFile["animation_interval"].is_number()) {
            Logger::error("SpriteAnimation::loadJson: Field 'animation_interval' must be a number");
            throw std::invalid_argument("SpriteAnimation::loadJson: Field 'animation_interval' must be a number");
        }

        // Load and validate texture
        std::string texture_id = jsonFile["texture_id"].get<std::string>();
        const sf::Texture* texture = ResourceManager::getInstance().getTexture(texture_id);
        if (texture == nullptr) {
            Logger::error("SpriteAnimation::loadJson: Failed to load texture with ID: " + texture_id);
            throw std::runtime_error("SpriteAnimation::loadJson: Failed to load texture with ID: " + texture_id);
        }

        // Extract and validate dimensions
        width = jsonFile["width"].get<int>();
        height = jsonFile["height"].get<int>();
        
        if (width <= 0) {
            Logger::error("SpriteAnimation::loadJson: Width must be positive, got: " + std::to_string(width));
            throw std::invalid_argument("SpriteAnimation::loadJson: Width must be positive, got: " + std::to_string(width));
        }
        if (height <= 0) {
            Logger::error("SpriteAnimation::loadJson: Height must be positive, got: " + std::to_string(height));
            throw std::invalid_argument("SpriteAnimation::loadJson: Height must be positive, got: " + std::to_string(height));
        }

        // Validate viewport object structure
        nlohmann::json viewport = jsonFile["viewport"];
        if (!viewport.contains("x")) {
            Logger::error("SpriteAnimation::loadJson: Viewport missing required field 'x'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport missing required field 'x'");
        }
        if (!viewport.contains("y")) {
            Logger::error("SpriteAnimation::loadJson: Viewport missing required field 'y'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport missing required field 'y'");
        }
        if (!viewport.contains("width")) {
            Logger::error("SpriteAnimation::loadJson: Viewport missing required field 'width'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport missing required field 'width'");
        }
        if (!viewport.contains("height")) {
            Logger::error("SpriteAnimation::loadJson: Viewport missing required field 'height'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport missing required field 'height'");
        }

        // Validate viewport field types
        if (!viewport["x"].is_number_integer()) {
            Logger::error("SpriteAnimation::loadJson: Viewport 'x' must be an integer");
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport 'x' must be an integer");
        }
        if (!viewport["y"].is_number_integer()) {
            Logger::error("SpriteAnimation::loadJson: Viewport 'y' must be an integer");
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport 'y' must be an integer");
        }
        if (!viewport["width"].is_number_integer()) {
            Logger::error("SpriteAnimation::loadJson: Viewport 'width' must be an integer");
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport 'width' must be an integer");
        }
        if (!viewport["height"].is_number_integer()) {
            Logger::error("SpriteAnimation::loadJson: Viewport 'height' must be an integer");
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport 'height' must be an integer");
        }

        // Extract viewport values and validate
        int viewport_x = viewport["x"].get<int>();
        int viewport_y = viewport["y"].get<int>();
        int viewport_width = viewport["width"].get<int>();
        int viewport_height = viewport["height"].get<int>();

        if (viewport_width <= 0) {
            Logger::error("SpriteAnimation::loadJson: Viewport width must be positive, got: " + std::to_string(viewport_width));
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport width must be positive, got: " + std::to_string(viewport_width));
        }
        if (viewport_height <= 0) {
            Logger::error("SpriteAnimation::loadJson: Viewport height must be positive, got: " + std::to_string(viewport_height));
            throw std::invalid_argument("SpriteAnimation::loadJson: Viewport height must be positive, got: " + std::to_string(viewport_height));
        }

        // Calculate number of frames and validate for division by zero
        sf::Vector2i numberOfFrame = {
            viewport_width / width,
            viewport_height / height};

        if (numberOfFrame.x <= 0) {
            Logger::error("SpriteAnimation::loadJson: Invalid frame calculation - viewport width (" + std::to_string(viewport_width) + 
                         ") must be greater than or equal to frame width (" + std::to_string(width) + ")");
            throw std::invalid_argument("SpriteAnimation::loadJson: Invalid frame calculation - insufficient viewport width for frame size");
        }
        if (numberOfFrame.y <= 0) {
            Logger::error("SpriteAnimation::loadJson: Invalid frame calculation - viewport height (" + std::to_string(viewport_height) + 
                         ") must be greater than or equal to frame height (" + std::to_string(height) + ")");
            throw std::invalid_argument("SpriteAnimation::loadJson: Invalid frame calculation - insufficient viewport height for frame size");
        }

        // Clear any existing sprites before loading new ones
        sprites.clear();

        // Create sprite frames
        for (int j = 0; j < numberOfFrame.y; j++) {
            for (int i = 0; i < numberOfFrame.x; i++) {
                sprites.push_back(sf::Sprite(
                    *texture, sf::IntRect{{viewport_x + i * width,
                                           viewport_y + j * height},
                                          {width, height}}));
            }
        }

        if (sprites.empty()) {
            Logger::error("SpriteAnimation::loadJson: No sprite frames were created");
            throw std::runtime_error("SpriteAnimation::loadJson: No sprite frames were created");
        }

        // Set sprite origins
        sf::Vector2f origin = sf::Vector2f{jsonFile["center_x"].get<float>(),
                                           jsonFile["center_y"].get<float>()};

        for (sf::Sprite& sprite : sprites) {
            sprite.setOrigin(origin);
        }

        // Validate and set animation type
        std::string animationType = jsonFile["type"].get<std::string>();
        if (animationType == "circular") {
            type = AnimationType::Circular;
        } else if (animationType == "linear") {
            type = AnimationType::Linear;
        } else {
            Logger::error("SpriteAnimation::loadJson: Invalid animation type '" + animationType + "'. Must be 'circular' or 'linear'");
            throw std::invalid_argument("SpriteAnimation::loadJson: Invalid animation type '" + animationType + "'. Must be 'circular' or 'linear'");
        }

        // Validate and set animation interval
        float animation_interval = jsonFile["animation_interval"].get<float>();
        if (animation_interval <= 0.0f) {
            Logger::error("SpriteAnimation::loadJson: Animation interval must be positive, got: " + std::to_string(animation_interval));
            throw std::invalid_argument("SpriteAnimation::loadJson: Animation interval must be positive, got: " + std::to_string(animation_interval));
        }

        // Initialize animation timer
        animationTimer = std::make_unique<Timer>();
        animationTimer->setTimeInterval(animation_interval)
            .setTimerMode(TimerMode::Continuous)
            .setRemainingTime(animation_interval);

        // Logger::info("SpriteAnimation::loadJson: Successfully loaded animation with " + std::to_string(sprites.size()) + " frames");

    } catch (const nlohmann::json::exception& e) {
        Logger::error("SpriteAnimation::loadJson: JSON parsing error - " + std::string(e.what()));
        throw std::runtime_error("SpriteAnimation::loadJson: JSON parsing error - " + std::string(e.what()));
    } catch (const std::exception& e) {
        Logger::error("SpriteAnimation::loadJson: Error loading animation - " + std::string(e.what()));
        throw; // Re-throw the existing exception
    }
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

void SpriteAnimation::restart() {
    animationTimer->reset();
    currentFrame = 0;
}

void SpriteAnimation::resume() { animationTimer->resume(); }

void SpriteAnimation::pause() { animationTimer->pause(); }

void SpriteAnimation::setCurrentFrame(int frame) {
    if (frame < 0 || frame >= sprites.size()) {
        Logger::error("SpriteAnimation::setCurrentFrame: Frame index out of range");
        throw std::out_of_range("SpriteAnimation::setCurrentFrame: Frame index out of range");
    }
    currentFrame = frame;
}

void SpriteAnimation::updateSpriteSize(float newWidth, float newHeight) {
    for (sf::Sprite& sprite : sprites) {
        sprite.setScale({newWidth / width, newHeight / height});
    }
}

void SpriteAnimation::setPosition(const sf::Vector2f& position) {
    for (sf::Sprite& sprite : sprites) {
        sprite.setPosition(position);
    }
}