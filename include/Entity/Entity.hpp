#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

/**
 * @brief Base class for all game entities
 * 
 * This class provides the common interface and properties for all game entities
 * including position, rotation, health, sprite rendering, and basic behaviors.
 */
class Entity {
protected:
    sf::Vector2f position;
    sf::Angle rotation;
    std::optional<sf::Sprite> sprite;
    bool alive;
    int entityId;
    static int nextEntityId;

public:
    /**
     * @brief Construct a new Entity object
     * 
     * @param pos Initial position
     */
    Entity(const sf::Vector2f& pos = sf::Vector2f(0, 0));

    /**
     * @brief Virtual destructor
     */
    virtual ~Entity() = default;

    /**
     * @brief Update the entity's state
     * 
     * @param deltaTime Time elapsed since last update
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Render the entity to the given render target
     * 
     * @param target Render target to draw on
     */
    virtual void render(sf::RenderTarget& target);

    /**
     * @brief Get the current position of the entity
     * 
     * @return sf::Vector2f Current position
     */
    sf::Vector2f getPosition() const { return position; }

    /**
     * @brief Set the position of the entity
     * 
     * @param pos New position
     */
    void setPosition(const sf::Vector2f& pos);

    /**
     * @brief Get the current rotation of the entity
     * 
     * @return sf::Angle Current rotation
     */
    sf::Angle getRotation() const { return rotation; }

    /**
     * @brief Set the rotation of the entity
     * 
     * @param rot New rotation
     */
    void setRotation(const sf::Angle& rot);

    /**
     * @brief Check if the entity is alive
     * 
     * @return bool True if alive, false otherwise
     */
    bool isAlive() const { return alive; }

    /**
     * @brief Set the alive state of the entity
     * 
     * @param isAlive New alive state
     */
    void setAlive(bool isAlive) { alive = isAlive; }

    /**
     * @brief Get the entity's unique ID
     * 
     * @return int Entity ID
     */
    int getEntityId() const { return entityId; }

    /**
     * @brief Set the sprite for this entity
     * 
     * @param newSprite The sprite to use
     */
    void setSprite(const sf::Sprite& newSprite);

    /**
     * @brief Create and set a sprite from a texture
     * 
     * @param texture The texture to use for the sprite
     */
    void setSpriteTexture(const sf::Texture& texture);

    /**
     * @brief Check if the entity has a sprite
     * 
     * @return bool True if sprite is set, false otherwise
     */
    bool hasSprite() const { return sprite.has_value(); }

    /**
     * @brief Get the sprite (if available)
     * 
     * @return const sf::Sprite* Pointer to sprite or nullptr if not set
     */
    const sf::Sprite* getSprite() const { 
        return sprite.has_value() ? &sprite.value() : nullptr; 
    }
};

#endif // ENTITY_HPP
