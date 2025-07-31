/**
 * @file TowerBuilder.hpp
 * @brief Declares the TowerBuilder class for constructing Tower entities with a fluent interface.
 *
 * The TowerBuilder uses the Builder pattern to create fully configured Tower objects
 * with proper validation and flexible configuration options.
 */
#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

#include "Entity/Tower/Tower.hpp"
#include "Entity/Tower/TowerStat.hpp"
#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Entity/Tower/Upgrades/UpgradeManager.hpp"
#include "Entity/Tower/Upgrades/UpgradeType.hpp"
#include "Gameplay/Currency.hpp"

class Scene;

/**
 * @class TowerBuilder
 * @brief Builder class for constructing Tower entities with a fluent interface.
 *
 * Provides a flexible way to construct towers step-by-step with proper validation.
 * Supports method chaining for easy configuration.
 */
class TowerBuilder {
private:
    // Required parameters
    std::string id;
    Scene* scene;
    
    // Optional parameters with defaults
    std::string name;
    std::string description;
    bool buildable;
    Currency cost;
    sf::Vector2f position;
    sf::Angle angle;
    
    // Asset paths for texture loading
    std::string baseTextureId;
    nlohmann::json turretAnimationPath;
    
    // Texture dimensions
    float textureWidth;
    float textureHeight;
    
    // Complex components
    std::unique_ptr<TowerStat> stats;
    std::vector<std::unique_ptr<TowerBehavior>> behaviors;
    
    // Upgrade system
    std::map<int, std::unique_ptr<UpgradeType>> upgradeTypes;
    int maxTotalUpgrades;
    
    // Timer configuration
    float timerInterval;
    
    // Build state
    bool isBuilt;

public:
    /**
     * @brief Construct a new TowerBuilder.
     */
    TowerBuilder();
    
    /**
     * @brief Reset the builder to initial state for reuse.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& reset();
    
    /**
     * @brief Set the tower's unique identifier (required).
     * @param id Unique identifier for the tower type.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setId(const std::string& id);
    
    /**
     * @brief Set the scene reference (required).
     * @param scene Reference to the game scene.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setScene(Scene& scene);
    
    /**
     * @brief Set the tower's display name.
     * @param name Display name of the tower.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setName(const std::string& name);
    
    /**
     * @brief Set the tower's description.
     * @param description Description of the tower's capabilities.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setDescription(const std::string& description);
    
    /**
     * @brief Set whether the tower is directly buildable.
     * @param buildable True if the tower can be built directly.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setBuildable(bool buildable);
    
    /**
     * @brief Set the cost required to build this tower.
     * @param cost Cost in game currencies.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setCost(const Currency& cost);
    
    /**
     * @brief Set the tower's initial position.
     * @param position Initial position in world coordinates.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setPosition(const sf::Vector2f& position);
    
    /**
     * @brief Set the tower's initial rotation angle.
     * @param angle Initial rotation angle.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setAngle(const sf::Angle& angle);
    
    /**
     * @brief Set the base texture path.
     * @param texturePath Path to the base texture file.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setBaseTexturePath(const std::string& texturePath);
    
    /**
     * @brief Set the turret animation path from a JSON section.
     * @param jsonFile JSON section containing the texture path.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setTurretAnimationPath(const nlohmann::json& jsonFile);

    /**
     * @brief Set the desired texture width.
     * @param width The width that textures should be scaled to.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setTextureWidth(float width);
    
    /**
     * @brief Set the desired texture height.
     * @param height The height that textures should be scaled to.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setTextureHeight(float height);
    
    /**
     * @brief Set both texture dimensions at once.
     * @param width The width that textures should be scaled to.
     * @param height The height that textures should be scaled to.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setTextureDimensions(float width, float height);
    
    /**
     * @brief Set the tower's statistics.
     * @param stats Unique pointer to tower statistics.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setStats(std::unique_ptr<TowerStat> stats);
    
    /**
     * @brief Add a behavior to the tower.
     * @param behavior Unique pointer to a tower behavior.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& addBehavior(std::unique_ptr<TowerBehavior> behavior);
    
    /**
     * @brief Set the timer interval for the tower's timer.
     * @param interval Timer interval in seconds.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setTimerInterval(float interval);
    
    /**
     * @brief Set the maximum total upgrades allowed for this tower.
     * @param maxUpgrades Maximum number of upgrades across all types.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& setMaxTotalUpgrades(int maxUpgrades);
    
    /**
     * @brief Add an upgrade type to the tower.
     * @param typeId Unique identifier for the upgrade type (1, 2, 3, etc.).
     * @param upgradeType Unique pointer to the upgrade type.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& addUpgradeType(int typeId, std::unique_ptr<UpgradeType> upgradeType);
    
    /**
     * @brief Create and add a simple upgrade type with basic configuration.
     * @param typeId Unique identifier for the upgrade type.
     * @param displayName Display name for the upgrade.
     * @param description Description of the upgrade.
     * @param maxLevel Maximum level for this upgrade type.
     * @param iconPath Path to the upgrade icon.
     * @param evolveTo Tower type to evolve to at max level.
     * @return TowerBuilder& Reference to this builder for chaining.
     */
    TowerBuilder& addSimpleUpgradeType(int typeId, const std::string& displayName, 
                                      const std::string& description, int maxLevel,
                                      const std::string& iconPath = "", 
                                      const std::string& evolveTo = "");
    
    /**
     * @brief Build and return the configured Tower.
     * @return std::unique_ptr<Tower> Newly created tower with all configurations applied.
     * @throws std::invalid_argument If required parameters are missing.
     */
    std::unique_ptr<Tower> build();
    
private:
    /**
     * @brief Validate that all required parameters are set.
     * @throws std::invalid_argument If validation fails.
     */
    void validate() const;
    
    /**
     * @brief Load textures and apply them to the tower.
     * @param tower Tower to apply textures to.
     */
    void loadTextures(Tower& tower) const;
};
