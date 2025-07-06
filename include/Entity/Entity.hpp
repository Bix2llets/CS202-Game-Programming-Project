#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

/**
 * @brief Base class for all game entities
 *
 * This class provides the common interface and properties for all game entities
 * including position, rotation, health, sprite rendering, and basic behaviors.
 */
class Scene;
class Entity : public sf::Drawable {
   protected:
    sf::Vector2f position;
    sf::Angle rotation;
    // ! Sprite should be loaded from texture, and keep intact during lifetime
    std::optional<sf::Sprite> sprite;
    static int nextEntityId;
    Scene& scene;

   public:
    const int entityId;
    /**
     * @brief Construct a new Entity object
     *
     * @param pos Initial position
     */
    Entity(const Scene &scene, const sf::Vector2f& position = sf::Vector2f(0, 0), const sf::Angle& rotation = sf::degrees(0.f));

    /**
     * @brief Virtual destructor
     */
    virtual ~Entity() = default;

    /**
     * @brief Update the entity's state
     */
    virtual void update() = 0;

    /**
     * @brief Render the entity to the given render target
     *
     * @param target Render target to draw on
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /**
     * @brief Get the current position of the entity
     *
     * @return sf::Vector2f Current position
     */
    inline sf::Vector2f getPosition() const { return position; }

    /**
     * @brief Set the position of the entity
     *
     * @param pos New position
     */
    virtual void setPosition(const sf::Vector2f& pos) = 0;

    /**
     * @brief Get the current rotation of the entity
     *
     * @return sf::Angle Current rotation
     */
    inline sf::Angle getRotation() const { return rotation; }

    /**
     * @brief Set the rotation of the entity
     *
     * @param rot New rotation
     */
    virtual void setRotation(const sf::Angle& rot) = 0;

    /**
     * @brief Create and set a sprite from a texture
     *
     * @param texture The texture to use for the sprite
     */
    void loadSpriteTexture(const sf::Texture& texture);
};
