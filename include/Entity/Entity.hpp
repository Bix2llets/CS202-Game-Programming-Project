/**
 * @file Entity.hpp
 * @brief Declares the Entity base class for all game entities.
 *
 * This class provides the common interface and properties for all game entities
 * including position, rotation, sprite rendering, and basic behaviors.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

class Scene;

/**
 * @class Entity
 * @brief Base class for all game entities.
 *
 * Provides position, rotation, and sprite management, as well as update and draw interfaces.
 */
class Entity : public sf::Drawable {
protected:
    sf::Vector2f position; ///< Position of the entity in world coordinates
    sf::Angle rotation;     ///< Rotation of the entity
    sf::Sprite sprite; ///< Optional sprite for rendering
    Scene& scene; ///< Reference to the scene this entity belongs to

public:
    /**
     * @brief Construct a new Entity object.
     * @param scene Reference to the scene this entity belongs to.
     */
    Entity(Scene &scene, const sf::Texture& texture) : scene(scene), sprite(texture) {}

    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~Entity() = default;

    /**
     * @brief Update the entity's state (pure virtual).
     * Implement entity-specific update logic in derived classes.
     */
    virtual void update() = 0;

    /**
     * @brief Render the entity to the given render target.
     * @param target Render target to draw on.
     * @param state Render states for drawing.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /**
     * @brief Get the current position of the entity.
     * @return sf::Vector2f Current position.
     */
    inline sf::Vector2f getPosition() const { return position; }

    /**
     * @brief Set the position of the entity (pure virtual).
     * @param pos New position.
     */
    virtual void setPosition(const sf::Vector2f& pos) = 0;

    /**
     * @brief Get the current rotation of the entity.
     * @return sf::Angle Current rotation.
     */
    inline sf::Angle getRotation() const { return rotation; }

    /**
     * @brief Set the rotation of the entity (pure virtual).
     * @param rot New rotation.
     */
    virtual void setRotation(const sf::Angle& rot) = 0;

    /**
     * @brief Create and set a sprite from a texture.
     * @param texture The texture to use for the sprite.
     */
    void changeSprite(const sf::Texture& texture);
};
