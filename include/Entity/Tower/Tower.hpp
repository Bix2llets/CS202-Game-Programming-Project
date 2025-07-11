/**
 * @file Tower.hpp
 * @brief Declares the Tower base class for all tower entities in the game.
 *
 * The Tower class provides the common interface and properties for all tower
 * types, including cost, cooldown management, and upgrade logic. Specific tower
 * types should inherit from this class and implement their own behavior and
 * rendering.
 */
#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Entity/Cooldown.hpp"
#include "Entity/Entity.hpp"
#include "Entity/Tower/TowerStat.hpp"
#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Gameplay/Currency.hpp"
#include "Base/Constants.hpp"

class Scene;
// enum class RoundEvent;

/**
 * @class Tower
 * @brief Abstract base class for all tower entities.
 *
 * Towers are defensive structures that can be placed in the game world. They
 * have a cost, can be upgraded, and have cooldowns for their actions. Derived
 * classes should implement specific attack or support behaviors.
 */
class Tower : public Entity {
private:
    // std::map<RoundEvent, std::function<void()>> roundEvents; ///< Event handlers for round events
    Cooldown timer;  ///< Cooldown timer for tower actions
    std::unique_ptr<TowerStat> stats;  ///< Pointer to tower statistics/attributes
    std::vector<std::unique_ptr<TowerBehavior>> behaviors; ///< List of behaviors associated with the tower
    
    // Tower identity and properties
    std::string id;          ///< Unique identifier for the tower type
    std::string name;        ///< Display name of the tower
    std::string description; ///< Description of the tower's capabilities
    bool buildable;          ///< Whether the tower can be directly built (if false, must be evolved)
    Currency cost;           ///< Cost required to build this tower

    // Dual sprite system
    sf::Sprite base;   ///< Base sprite with independent rotation
    sf::Angle baseRotation;          ///< Rotation angle for the base sprite
    
    // Texture dimensions
    float textureWidth;      ///< Desired width for tower textures
    float textureHeight;     ///< Desired height for tower textures

public:
    /**
     * @brief Construct a new Tower object.
     * @param scene Reference to the game scene.
     * @param id Unique identifier for the tower type.
     * @param pos Position to place the tower.
     * @param angle Initial rotation angle.
     */
    Tower(Scene& scene, const std::string& id, const sf::Vector2f& pos = sf::Vector2f(0, 0),
          const sf::Angle& angle = sf::radians(0.f))
        : Entity(scene), id(id), name(""), description(""), buildable(true), cost(0, 0), 
          base(GameConstants::BLANK_TEXTURE), baseRotation(sf::radians(0.f)), textureWidth(32.0f), textureHeight(32.0f) {
        setPosition(pos);
        setRotation(angle);
    }

    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    ~Tower() override = default;

    /**
     * @brief Update the tower's behavior.
     * Handles cooldowns, targeting, and attacks.
     */
    void update();

    /**
     * @brief Render the tower.
     * @param target Render target to draw on.
     * @param state Current render states.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    /**
     * @brief Update both base sprite and turret sprite positions.
     * Overrides Entity::setPosition to handle dual sprite system.
     * @param pos New position.
     */
    void setPosition(const sf::Vector2f& pos) override;

    /**
     * @brief Set the rotation of the turret (sprite from Entity).
     * Overrides Entity::setRotation to control turret rotation.
     * @param rot New turret rotation.
     */
    void setRotation(const sf::Angle& rot) override;

    /**
     * @brief Get the current rotation of the base.
     * @return sf::Angle Current base rotation.
     */
    sf::Angle getBaseRotation() const { return baseRotation; }

    /**
     * @brief Set the rotation of the base.
     * @param rot New base rotation.
     */
    void setBaseRotation(const sf::Angle& rot);

    /**
     * @brief Get the current rotation of the turret (sprite from Entity).
     * @return sf::Angle Current turret rotation.
     */
    sf::Angle getTurretRotation() const { return rotation; }

    /**
     * @brief Set the rotation of the turret.
     * @param rot New turret rotation.
     */
    void setTurretRotation(const sf::Angle& rot);

    /**
     * @brief Create and set the base sprite from a texture.
     * @param texture The texture to use for the base sprite.
     */
    void loadBaseSpriteTexture(const sf::Texture& texture);

    /**
     * @brief Create and set the turret sprite from a texture.
     * This is an alias for loadSpriteTexture from Entity class.
     * @param texture The texture to use for the turret sprite.
     */
    void loadTurretSpriteTexture(const sf::Texture& texture);

    /**
     * @brief Add a new behavior to the tower.
     * This allows the tower to have multiple behaviors.
     * @param behavior 
     */
    void addBehavior(std::unique_ptr<TowerBehavior> behavior);

    // Getters

    /**
     * @brief Get the tower's unique identifier.
     * @return const std::string& Tower type identifier.
     */
    const std::string& getId() const { return id; }

    /**
     * @brief Get the tower's display name.
     * @return const std::string& Tower name.
     */
    const std::string& getName() const { return name; }

    /**
     * @brief Get the tower's description.
     * @return const std::string& Tower description.
     */
    const std::string& getDescription() const { return description; }

    /**
     * @brief Check if the tower is directly buildable.
     * @return bool True if the tower can be built directly, false if it must be evolved.
     */
    bool isBuildable() const { return buildable; }

    /**
     * @brief Get the cost required to build this tower.
     * @return const Currency& Cost in game currencies.
     */
    const Currency& getCost() const { return cost; }

    /**
     * @brief Get the tower's cooldown timer.
     * @return Reference to the cooldown timer.
     */
    const Cooldown& getTimer() const { return timer; }

    /**
     * @brief Get the tower's cooldown timer (non-const).
     * @return Reference to the cooldown timer.
     */
    Cooldown& getTimer() { return timer; }

    /**
     * @brief Get the tower's statistics.
     * @return Pointer to the tower statistics.
     */
    const TowerStat* getStats() const;

    /**
     * @brief Get the tower's statistics (non-const).
     * @return Pointer to the tower statistics.
     */
    TowerStat* getStats();
    
    /**
     * @brief Get a specific statistic value (with bonus) by name.
     * @param statName Name of the statistic to retrieve.
     * @param defaultValue Default value to return if the statistic is not found.
     * @return The value of the specified statistic, or defaultValue if not found.
     */
    float getStat(const std::string& statName, float defaultValue = 0.0f) const;
    
    /**
     * @brief Get the desired texture width.
     * @return float The width that textures should be scaled to.
     */
    float getTextureWidth() const { return textureWidth; }
    
    /**
     * @brief Get the desired texture height.
     * @return float The height that textures should be scaled to.
     */
    float getTextureHeight() const { return textureHeight; }
    
    // Setters
    /**
     * @brief Set the tower's display name.
     * @param newName New name for the tower.
     */
    void setName(const std::string& newName) { name = newName; }

    /**
     * @brief Set the tower's description.
     * @param newDescription New description for the tower.
     */
    void setDescription(const std::string& newDescription) { description = newDescription; }

    /**
     * @brief Set whether the tower is directly buildable.
     * @param canBuild True if the tower can be built directly, false if it must be evolved.
     */
    void setBuildable(bool canBuild) { buildable = canBuild; }

    /**
     * @brief Set the cost required to build this tower.
     * @param newCost New cost in game currencies.
     */
    void setCost(const Currency& newCost) { cost = newCost; }

    /**
     * @brief Set the tower's cooldown timer interval.
     * @param interval The new cooldown interval.
     */
    void setTimerInterval(float interval);

    /**
     * @brief Set the desired texture width.
     * @param width The width that textures should be scaled to.
     */
    void setTextureWidth(float width) { textureWidth = width; }
    
    /**
     * @brief Set the desired texture height.
     * @param height The height that textures should be scaled to.
     */
    void setTextureHeight(float height) { textureHeight = height; }
    
    /**
     * @brief Set both texture dimensions at once.
     * @param width The width that textures should be scaled to.
     * @param height The height that textures should be scaled to.
     */
    void setTextureDimensions(float width, float height) { 
        textureWidth = width; 
        textureHeight = height; 
    }

    /**
     * @brief Initialize tower statistics.
     * @param newStats Unique pointer to new tower statistics.
     */
    void setStats(std::unique_ptr<TowerStat> newStats);
};
