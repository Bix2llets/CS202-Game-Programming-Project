
class Level;
class Scene;
class Enemy;
class TowerBehavior;
class CombatBehavior;
class ResourceBehavior;
class GlowingBehavior;
enum class BehaviorType;

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
#include "Entity/Modules/Timer.hpp"
#include "Entity/Modules/SpriteAnimation.hpp"
#include "Entity/Entity.hpp"
#include "Entity/Tower/TowerStat.hpp"
#include "Entity/Tower/Upgrades/UpgradeManager.hpp"
#include "Entity/Tower/Upgrades/UpgradeType.hpp"
#include "Gameplay/Currency.hpp"
#include "Base/Constants.hpp"

/**
 * @class Tower
 * @brief Abstract base class for all tower entities.
 *
 * Towers are defensive structures that can be placed in the game world. They
 * have a cost, can be upgraded, and have cooldowns for their actions. Derived
 * classes should implement specific attack or support behaviors.
 */
class Tower : public Entity {

friend class TowerBuilder; ///< Allow TowerBuilder to access private members
friend class UpgradeManager; ///< Allow UpgradeManager to access private members

private:
    Timer timer;  ///< Timer for tower actions
    std::unique_ptr<TowerStat> stats;  ///< Pointer to tower statistics/attributes
    std::unique_ptr<CombatBehavior> combatBehaviorPointer;
    std::unique_ptr<ResourceBehavior> resourceBehaviorPointer;
    std::unique_ptr<GlowingBehavior> glowingBehaviorPointer;
    std::unique_ptr<UpgradeManager> upgradeManager; ///< Manager for tower upgrades
    Level* levelRef = nullptr; ///< Reference to Level if scene is a Level, else nullptr

    // Tower identity and properties
    std::string id;          ///< Unique identifier for the tower type
    std::string name;        ///< Display name of the tower
    std::string description; ///< Description of the tower's capabilities
    bool buildable;          ///< Whether the tower can be directly built (if false, must be evolved)
    Currency cost;           ///< Cost required to build this tower

    // Dual sprite system
    sf::Sprite base;   ///< Base sprite with independent rotation
    sf::Angle baseRotation;          ///< Rotation angle for the base sprite
    SpriteAnimation turretAnimation; ///< Animation for the turret sprite (originally from Entity)

    sf::RenderTexture iconRenderTexture; ///< Render texture for the tower icon
    sf::Sprite icon; ///< Non-animated sprite of the tower for UI display

    // Texture dimensions
    float textureWidth;      ///< Desired width for tower textures
    float textureHeight;     ///< Desired height for tower textures

    // Target tracking
    Enemy* mainTarget;       ///< Current main target enemy for barrel tracking

public:
    /**
     * @brief Construct a new Tower object.
     * @param scene Reference to the game scene.
     * @param id Unique identifier for the tower type.
     * @param pos Position to place the tower.
     * @param angle Initial rotation angle.
     */
    Tower(Scene& scene, const std::string& id, const sf::Vector2f& pos = sf::Vector2f(0, 0),
          const sf::Angle& angle = sf::radians(0.f));

    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    ~Tower() override;

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
     * @brief Point the turret toward a specific position.
     * Calculates the angle from the tower's position to the target position
     * and rotates the turret to face that direction.
     * @param targetPosition The position to point the turret toward.
     */
    void pointTurretTowards(const sf::Vector2f& targetPosition);

    /**
     * @brief Create and set the base sprite from a texture.
     * @param texture The texture to use for the base sprite.
     */
    void loadBaseSpriteTexture(const sf::Texture& texture);

    /**
     * @brief Create and set the turret sprite and animations from turretAnimationPath.
     */
    void loadTurretSpriteAnimation(const nlohmann::json& turretAnimationPath);

    /**
     * @brief Update the turret animation based on the current state.
     */
    void updateSpriteTurretAnimation();

    /**
     * @brief Load the tower's icon sprite for GUI display.
     */
    void loadIcon();

    /**
     * @brief Get the icon sprite for GUI display.
     * @return sf::Sprite Icon sprite representing the tower for UI.
     */
    sf::Sprite getIcon() const { return icon; }

    /**
     * @brief Add a new behavior to the tower.
     * Behaviors are placed in fixed slots: [0]=Combat, [1]=Resource, [2]=Glowing.
     * @param behavior Unique pointer to the behavior to add.
     */
    void addBehavior(std::unique_ptr<TowerBehavior> behavior);
    void removeBehavior(BehaviorType type);

    /**
     * @brief Get the Combat Behavior object
     * This method returns the combat behavior if it exists.
     * @return CombatBehavior* 
     */
    CombatBehavior* getCombatBehavior() const { return combatBehaviorPointer.get(); }

    /**
     * @brief Get the Resource Behavior object
     * This method returns the resource behavior if it exists.
     * @return ResourceBehavior* 
     */
    ResourceBehavior* getResourceBehavior() const { return resourceBehaviorPointer.get(); }
    
    /**
     * @brief Get the Glowing Behavior object
     * This method returns the glowing behavior if it exists.
     * @return GlowingBehavior* 
     */
    GlowingBehavior* getGlowingBehavior() const { return glowingBehaviorPointer.get(); }
    
    /**
     * @brief Get the level reference if available.
     * @return Reference to the level.
     * @throws std::runtime_error if the scene is not a Level.
     */
    Level* getLevelRef() const { return levelRef; }

    /**
     * @brief Check if the tower is currently in a Level.
     * @return true if the tower is in a Level, false otherwise.
     */
    bool isInLevel() const {
        return levelRef != nullptr;
    }

    // Upgrade System Methods

    /**
     * @brief Set the maximum total upgrades allowed for this tower.
     * @param maxUpgrades Maximum total upgrade levels across all types.
     */
    void setMaxTotalUpgrades(int maxUpgrades);

    /**
     * @brief Add an upgrade type to this tower.
     * @param typeId Unique identifier for the upgrade type.
     * @param upgradeType Upgrade type to add.
     */
    void addUpgradeType(int typeId, std::unique_ptr<UpgradeType> upgradeType);

    /**
     * @brief Attempt to upgrade a specific upgrade type.
     * @param upgradeTypeId The upgrade type ID to upgrade.
     * @param playerCurrency Reference to player's currency (will be modified if upgrade succeeds).
     * @return Result of the upgrade attempt.
     */
    UpgradeResult attemptUpgrade(int upgradeTypeId, Currency& playerCurrency);

    /**
     * @brief Check if an upgrade type can be upgraded.
     * @param upgradeTypeId The upgrade type ID.
     * @param playerCurrency Player's current currency.
     * @return True if the upgrade is possible.
     */
    bool canUpgrade(int upgradeTypeId, const Currency& playerCurrency) const;

    /**
     * @brief Get the cost for the next level of an upgrade type.
     * @param upgradeTypeId The upgrade type ID.
     * @return Pointer to upgrade details for next level, or nullptr if not possible.
     */
    const UpgradeDetails* getNextUpgradeCost(int upgradeTypeId) const;

    /**
     * @brief Get the current level of a specific upgrade type.
     * @param upgradeTypeId The upgrade type ID.
     * @return Current level (0 if not found or no upgrades).
     */
    int getUpgradeLevel(int upgradeTypeId) const;

    /**
     * @brief Get available evolution options for this tower.
     * @return Vector of tower types that this tower can evolve to.
     */
    std::vector<std::string> getAvailableEvolutions() const;

    /**
     * @brief Get the upgrade manager (const).
     * @return Pointer to the upgrade manager.
     */
    const UpgradeManager* getUpgradeManager() const { return upgradeManager.get(); }

    /**
     * @brief Get the upgrade manager (non-const).
     * @return Pointer to the upgrade manager.
     */
    UpgradeManager* getUpgradeManager() { return upgradeManager.get(); }

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
     * @brief Get the tower's timer.
     * @return Reference to the timer.
     */
    const Timer& getTimer() const { return timer; }

    /**
     * @brief Get the tower's timer (non-const).
     * @return Reference to the timer.
     */
    Timer& getTimer() { return timer; }

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
     * @brief Get a specific statistic value (with upgrade bonuses) by name.
     * @param statName Name of the statistic to retrieve.
     * @param defaultValue Default value to return if the statistic is not found.
     * @return The value of the specified statistic including upgrade bonuses, or defaultValue if not found.
     */
    float getStat(const std::string& statName, float defaultValue = 0.0f) const;
    
    /**
     * @brief Get base statistic value (without upgrade bonuses) by name.
     * @param statName Name of the statistic to retrieve.
     * @param defaultValue Default value to return if the statistic is not found.
     * @return The base value of the specified statistic, or defaultValue if not found.
     */
    float getBaseStat(const std::string& statName, float defaultValue = 0.0f) const;
    
    /**
     * @brief Check if tower has combat behavior.
     * @return True if tower has combat behavior.
     */
    bool hasCombatBehavior() const { return combatBehaviorPointer != nullptr; }

    /**
     * @brief Check if tower has resource behavior.
     * @return True if tower has resource behavior.
     */
    bool hasResourceBehavior() const { return resourceBehaviorPointer != nullptr; }

    /**
     * @brief Check if tower has glowing behavior.
     * @return True if tower has glowing behavior.
     */
    bool hasGlowingBehavior() const { return glowingBehaviorPointer != nullptr; }
    
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
    
    /**
     * @brief Get the current main target enemy.
     * @return Pointer to the main target enemy, or nullptr if no target is set.
     */
    Enemy* getMainTarget() const { return mainTarget; }
    
    /**
     * @brief Check if the tower has a main target.
     * @return True if the tower has a main target, false otherwise.
     */
    bool hasMainTarget() const { return mainTarget != nullptr; }
    
    // Setters

    /**
     * @brief Set the main target enemy for the tower.
     * @param target Pointer to the enemy to target, or nullptr to clear the target.
     */
    void setMainTarget(Enemy* target) { mainTarget = target; }

    /**
     * @brief Initialize tower statistics.
     * @param newStats Unique pointer to new tower statistics.
     */
    void setStats(std::unique_ptr<TowerStat> newStats);
};
