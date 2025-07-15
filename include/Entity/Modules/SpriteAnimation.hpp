
/**
 * @file SpriteAnimation.hpp
 * @brief Defines the SpriteAnimation class for handling sprite-based
 * animations.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <memory>

#include "Core/ResourceManager.hpp"
#include "Entity/Modules/Timer.hpp"

/**
 * @enum AnimationType
 * @brief Types of animation supported by SpriteAnimation.
 */
enum class AnimationType {
    Linear,   ///< Linear animation (frame by frame)
    Circular  ///< Circular animation (looping)
};

/**
 * @class SpriteAnimation
 * @brief Handles sprite-based animations using a sequence of frames.
 *
 * Supports linear and circular animation types, frame management, and
 * timer-based updates.
 */
class SpriteAnimation {
    friend class SpriteAnimationBuilder;

   private:
    AnimationType type;               ///< Animation type (linear or circular)
    sf::Vector2i spriteSize;          ///< Size of each sprite frame
    int currentFrame;                 ///< Current frame index
    std::vector<sf::Sprite> sprites;  ///< List of sprite frames
    std::unique_ptr<Timer>
        animationTimer;  ///< Timer for animation frame updates

   public:
    /**
     * @brief Default constructor for SpriteAnimation.
     */
    SpriteAnimation();

    /**
     * @brief Loads animation data from a JSON file and resource manager.
     * @param resManager Reference to the resource manager for loading textures.
     * @param jsonFile JSON object containing animation data.
     */
    void loadJson(ResourceManager& resManager, const nlohmann::json& jsonFile);

    /**
     * @brief Updates the animation timer and frame.
     */
    void update();

    /**
     * @brief Gets the current sprite for rendering.
     * @return The current sf::Sprite frame.
     */
    sf::Sprite getCurrentSprite();

    /**
     * @brief Restarts the animation from the first frame.
     */
    void restart();

    /**
     * @brief Continue playing the animation from the current frame
     */
    void resume();

    /**
     * @brief Stop playing the animation aT the current frame
     */
    void pause();
};