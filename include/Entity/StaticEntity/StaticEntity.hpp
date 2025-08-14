/**
 * @file StaticEntity.hpp
 * @brief Declares the StaticEntity class for static entities that can be removed with a cost.
 *
 * StaticEntity represents a game entity that doesn't move but has a texture,
 * dimensions, and a currency cost to remove it from the game.
 */
#pragma once

#include "Entity/Entity.hpp"
#include "Gameplay/Currency.hpp"
#include <SFML/Graphics.hpp>

class Level;

/**
 * @class StaticEntity
 * @brief A static entity that has texture, dimensions, and removal cost.
 *
 * This class represents entities that remain stationary but can be removed
 * by paying a specific currency cost. Examples could include obstacles,
 * decorations, or removable terrain features.
 */
class StaticEntity : public Entity {
private:
    std::string id;
    std::string name;

    Level* levelRef;
    float textureWidth;  ///< Width of the entity's texture
    float textureHeight; ///< Height of the entity's texture
    Currency removeCost; ///< Cost required to remove this entity

    friend class StaticEntityFactory;

public:
    /**
     * @brief Construct a new StaticEntity object.
     * @param scene Reference to the scene this entity belongs to.
     * @param texture The texture for this static entity.
     * @param width Width of the texture.
     * @param height Height of the texture.
     * @param cost Currency cost to remove this entity.
     */
    StaticEntity(Scene& scene);

    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~StaticEntity() = default;

    /**
     * @brief Update the entity's state.
     * Static entities typically don't need to update, but this can be overridden.
     */
    void update() override;

    /**
     * @brief Set the position of the entity.
     * @param pos New position.
     */
    void setPosition(const sf::Vector2f& pos) override;

    /**
     * @brief Set the rotation of the entity.
     * @param rot New rotation.
     */
    void setRotation(const sf::Angle& rot) override;

    /**
     * @brief Draw the entity to the target.
     * @param target The render target to draw on.
     * @param state The render states to apply.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /**
     * @brief Get the width of the entity's texture.
     * @return float Texture width.
     */
    float getTextureWidth() const { return textureWidth; }

    /**
     * @brief Get the height of the entity's texture.
     * @return float Texture height.
     */
    float getTextureHeight() const { return textureHeight; }

    /**
     * @brief Set the width of the entity's texture.
     * @param width New texture width.
     */
    void setTextureWidth(float width);

    /**
     * @brief Set the height of the entity's texture.
     * @param height New texture height.
     */
    void setTextureHeight(float height);

    void loadSpriteTexture(const sf::Texture& texture);

    /**
     * @brief Get the currency cost to remove this entity.
     * @return const Currency& Reference to the removal cost.
     */
    const Currency& getRemoveCost() const { return removeCost; }

    /**
     * @brief Set the currency cost to remove this entity.
     * @param cost New removal cost.
     */
    void setRemoveCost(const Currency& cost);

    /**
     * @brief Check if the entity can be removed with the given currency.
     * @param availableCurrency The currency available for spending.
     * @return bool True if the entity can be removed, false otherwise.
     */
    bool canBeRemoved(const Currency& availableCurrency) const;

    bool remove(Currency& availableCurrency) {
        if (canBeRemoved(availableCurrency)) {
            availableCurrency -= removeCost;
            return true;
        }
        return false;
    }

    /**
     * @brief Get the bounds of this entity.
     * @return sf::FloatRect The bounding rectangle of the entity.
     */
    sf::FloatRect getBounds() const;

    std::string getId() const { return id; }
    std::string getName() const { return name; }
};
