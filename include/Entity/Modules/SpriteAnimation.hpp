
/**
 * @file SpriteAnimation.hpp
 * @brief Defines the SpriteAnimation class for handling sprite-based
 * animations.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <memory>

#include "Entity/Modules/Timer.hpp"

class Tower;

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
    int currentFrame;                 ///< Current frame index
    std::vector<sf::Sprite> sprites;  ///< List of sprite frames
    std::unique_ptr<Timer> animationTimer;  ///< Timer for animation frame updates

    int width; ///< Width of each sprite frame
    int height; ///< Height of each sprite frame

public:
    /**
     * @brief Default constructor for SpriteAnimation.
     */
    SpriteAnimation();

    /**
     * @brief Copy constructor for SpriteAnimation.
     * @param other The SpriteAnimation to copy from.
     */
    SpriteAnimation(const SpriteAnimation& other)
        : type(other.type),
          currentFrame(other.currentFrame),
          sprites(other.sprites),
          animationTimer(std::make_unique<Timer>(*other.animationTimer)),
          width(other.width),
          height(other.height) {}

    /**
     * @brief Gets the current animation type.
     * @return The current AnimationType.
     */
    int getCurrentFrame() const { return currentFrame; }

    /**
     * @brief Gets the total number of frames in the animation.
     * @return The number of frames.
     */
    int getFrameCount() const { return static_cast<int>(sprites.size()); }

    /**
     * @brief Gets the current sprite for rendering.
     * @return The current sf::Sprite frame.
     */
    sf::Sprite getCurrentSprite() const { return sprites[currentFrame]; }

    /**
     * @brief Gets the width of each sprite frame.
     * @return The width of the sprite frame.
     */
    int getWidth() const { return width; }

    /**
     * @brief Gets the height of each sprite frame.
     * @return The height of the sprite frame.
     */
    int getHeight() const { return height; }

    /**
     * @brief Gets the size of the sprite frame.
     * @return sf::Vector2u containing width and height.
     */
    sf::Vector2u getSpriteSize() const { return sf::Vector2u(width, height); }

    /**
     * @brief Loads animation data from a JSON file and resource manager.
     * @param resManager Reference to the resource manager for loading textures.
     * @param jsonFile JSON object containing animation data.
     */
    void loadJson(const nlohmann::json& jsonFile);

    /**
     * @brief Updates the animation timer and frame.
     */
    void update();


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

    inline bool isRunning() {return animationTimer->isRunning();}

    /**
     * @brief Sets the current frame to a specific index.
     * @param frame The index of the frame to set.
     * @throws std::out_of_range if the frame index is invalid.
     */
    void setCurrentFrame(int frame);

    /**
     * @brief Updates the sprite size based on the tower's size.
     * @param tower Pointer to the Tower object.
     */
    void updateSpriteSize(float newWidth, float newHeight);

    /**
     * @brief Sets the position of the sprite animation.
     * @param position The new position to set.
     */
    void setPosition(const sf::Vector2f& position);

    /**
     * @brief Gets a reference to the vector of sprite frames.
     * @return A reference to the vector of sf::Sprite frames.
     */
    std::vector<sf::Sprite>& getSprites() { return sprites; }
};