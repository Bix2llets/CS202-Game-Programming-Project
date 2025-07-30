#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Base/Constants.hpp"
#include "Entity/Tower/Tower.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Utility/MathUtils.hpp"
#include <cmath>
#include <algorithm>
#include <iostream> // Include for debug output
#include <stdexcept>
#include "Scene/Scene.hpp"
#include "Scene/Level.hpp"

Tower::Tower(Scene& scene, const std::string& id, const sf::Vector2f& pos, const sf::Angle& angle)
    : Entity(scene), id(id), name(""), description(""), buildable(true), cost(0, 0),
      base(GameConstants::BLANK_TEXTURE), baseRotation(sf::radians(0.f)), textureWidth(32.0f), textureHeight(32.0f),
      mainTarget(nullptr) {
    upgradeManager = std::make_unique<UpgradeManager>(this);
    setPosition(pos);
    setRotation(angle);
    levelRef = dynamic_cast<Level*>(&scene);
    timer.resume();
}

Tower::~Tower() = default;

void Tower::addBehavior(std::unique_ptr<TowerBehavior> behavior) {
    if (!behavior) return;
    Tower* existingTower = behavior->getTower();
    if (existingTower != nullptr && existingTower != this) {
        throw std::runtime_error("Cannot add behavior to tower - behavior already belongs to another tower");
    }
    behavior->setTower(this);
    switch (behavior->getType()) {
        case BehaviorType::Combat:
            combatBehaviorPointer.reset(reinterpret_cast<CombatBehavior*>(behavior.release()));
            break;
        case BehaviorType::Resource:
            resourceBehaviorPointer.reset(reinterpret_cast<ResourceBehavior*>(behavior.release()));
            break;
        case BehaviorType::Glowing:
            glowingBehaviorPointer.reset(reinterpret_cast<GlowingBehavior*>(behavior.release()));
            break;
    }
}

void Tower::removeBehavior(BehaviorType type) {
    switch (type) {
        case BehaviorType::Combat:
            combatBehaviorPointer.reset();
            break;
        case BehaviorType::Resource:
            resourceBehaviorPointer.reset();
            break;
        case BehaviorType::Glowing:
            glowingBehaviorPointer.reset();
            break;
    }
}

const TowerStat* Tower::getStats() const {
    return stats.get();
}

TowerStat* Tower::getStats() {
    return stats.get();
}

float Tower::getStat(const std::string& statName, float defaultValue) const {
    // Get base stat with any multipliers
    float baseStat = stats ? stats->getStatWithBonus(statName, defaultValue) : defaultValue;
    
    // Add upgrade bonuses
    float upgradeBonus = upgradeManager ? upgradeManager->getTotalStatBonus(statName) : 0.0f;
    
    return baseStat + upgradeBonus;
}

float Tower::getBaseStat(const std::string& statName, float defaultValue) const {
    return stats ? stats->getStatWithBonus(statName, defaultValue) : defaultValue;
}

// Upgrade System Methods
UpgradeResult Tower::attemptUpgrade(int upgradeTypeId, Currency& playerCurrency) {
    return upgradeManager ? upgradeManager->attemptUpgrade(upgradeTypeId, playerCurrency) : UpgradeResult::InvalidUpgradeType;
}

bool Tower::canUpgrade(int upgradeTypeId, const Currency& playerCurrency) const {
    return upgradeManager ? upgradeManager->canUpgrade(upgradeTypeId, playerCurrency) : false;
}

const UpgradeDetails* Tower::getNextUpgradeCost(int upgradeTypeId) const {
    return upgradeManager ? upgradeManager->getNextUpgradeCost(upgradeTypeId) : nullptr;
}

int Tower::getUpgradeLevel(int upgradeTypeId) const {
    return upgradeManager ? upgradeManager->getCurrentLevel(upgradeTypeId) : 0;
}

std::vector<std::string> Tower::getAvailableEvolutions() const {
    return upgradeManager ? upgradeManager->getAvailableEvolutions() : std::vector<std::string>{};
}

void Tower::setMaxTotalUpgrades(int maxUpgrades) {
    if (upgradeManager) {
        upgradeManager->setMaxTotalUpgrades(maxUpgrades);
    }
}

void Tower::addUpgradeType(int typeId, std::unique_ptr<UpgradeType> upgradeType) {
    if (upgradeManager && upgradeType) {
        upgradeManager->addUpgradeType(typeId, std::move(upgradeType));
    }
}

void Tower::setStats(std::unique_ptr<TowerStat> newStats) {
    stats = std::move(newStats);
}

// Implementation of dual sprite system methods
void Tower::setPosition(const sf::Vector2f& pos) {
    position = pos;
    
    // Update base sprite position if it exists
    base.setPosition(pos);
    
    // Update turret sprite position if it exists
    sprite.setPosition(pos);
}

void Tower::setRotation(const sf::Angle& rot) {
    setTurretRotation(rot);
}

void Tower::setBaseRotation(const sf::Angle& rot) {
    baseRotation = rot;
    
    // Update base sprite rotation if it exists
    base.setRotation(rot);
}

void Tower::setTurretRotation(const sf::Angle& rot) {
    rotation = rot;  // Update Entity's rotation
    
    // Update turret sprite rotation if it exists
    sprite.setRotation(rot);
}

void Tower::pointTurretTowards(const sf::Vector2f& targetPosition) {
    setTurretRotation((targetPosition - position).angle() + sf::degrees(90));
}

void Tower::loadBaseSpriteTexture(const sf::Texture& texture) {
    base = sf::Sprite(texture);
    
    // Get original texture size
    sf::Vector2u originalSize = texture.getSize();
    
    // Set origin to center of ORIGINAL texture size (before scaling)
    sf::Vector2f origin(originalSize.x / 2.0f, originalSize.y / 2.0f);
    base.setOrigin(origin);
    
    // Scale sprite to desired dimensions
    float scaleX = textureWidth / static_cast<float>(originalSize.x);
    float scaleY = textureHeight / static_cast<float>(originalSize.y);
    base.setScale(sf::Vector2f(scaleX, scaleY));
    
    // Ensure position and rotation are set correctly after scaling
    base.setPosition(position);
    base.setRotation(baseRotation);  // Use the stored base rotation
}

void Tower::loadTurretSpriteTexture(const sf::Texture& texture) {
    // loadSpriteTexture(texture);  // Use Entity's method
    sprite = turretAnimation.getCurrentSprite();

    // Get original texture size
    sf::Vector2u originalSize = turretAnimation.getSpriteSize();
    
    // Set origin to center of ORIGINAL texture size (before scaling)
    sprite.setOrigin(sf::Vector2f(originalSize.x / 2.0f, originalSize.y / 2.0f));
    
    // Scale sprite to desired dimensions
    float scaleX = textureWidth / static_cast<float>(originalSize.x);
    float scaleY = textureHeight / static_cast<float>(originalSize.y);
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    
    // Ensure position and rotation are set correctly after scaling
    sprite.setPosition(position);
    sprite.setRotation(rotation);  // Use Entity's rotation for turret
}

void Tower::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    // Draw base sprite first (if it exists)
    target.draw(base, state);
    
    // Draw turret sprite on top (if it exists)
    target.draw(sprite, state);
}

void Tower::update() {
    if (!levelRef) return;
    
    timer.update();
    turretAnimation.update();

    // Combat behavior
    if (combatBehaviorPointer) {
        if (timer.isAvailable()) {
            std::vector<Enemy*> targets = levelRef->getEntityManager().getEnemies();
            if(targets.empty()) return; // No targets to engage
            
            
            combatBehaviorPointer->engage(targets);
            turretAnimation.restart();

            float fireRate = getStat(TowerStat::FIRE_RATE, 1.0f);
            float interval = (fireRate > 0.0f) ? (1.0f / fireRate) : 1.0f;
            timer.reset();
        }
        
        if (mainTarget) {
            pointTurretTowards(mainTarget->getPosition());
        }
    }

    loadTurretSpriteTexture(turretAnimation.getCurrentSprite().getTexture());
}