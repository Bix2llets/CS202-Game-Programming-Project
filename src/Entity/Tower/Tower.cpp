#include "Entity/Tower/Tower.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>  // Include for debug output
#include <stdexcept>

#include "Base/Constants.hpp"
#include "Entity/Enemy/Enemy.hpp"
#include "Entity/Tower/Behaviors/TowerBehavior.hpp"
#include "Scene/Level.hpp"
#include "Scene/Scene.hpp"
#include "Utility/CollisionChecker.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/logger.hpp"

Tower::Tower(Scene& scene, const std::string& id, const sf::Vector2f& pos,
             const sf::Angle& angle)
    : Entity(scene),
      id(id),
      name(""),
      description(""),
      buildable(true),
      cost(0, 0),
      base(GameConstants::BLANK_TEXTURE),
      baseRotation(sf::radians(0.f)),
      textureWidth(32.0f),
      textureHeight(32.0f),
      icon(GameConstants::BLANK_TEXTURE),
      mainTarget(nullptr) {
    upgradeManager = std::make_unique<UpgradeManager>(this);
    setPosition(pos);
    setRotation(angle);
    // Assign uniqueId only after levelRef is determined
    if (levelRef) {
        uniqueId = static_cast<int64_t>(
            levelRef->getRandom(RandomType::EntityID).nextU64());
    } else {
        // Fallback: use address-based id when outside a Level context
        uniqueId = static_cast<int64_t>(reinterpret_cast<std::uintptr_t>(this));
    }
    timer.resume();
}

void Tower::addBehavior(std::unique_ptr<TowerBehavior> behavior) {
    if (!behavior) return;
    Tower* existingTower = behavior->getTower();
    if (existingTower != nullptr && existingTower != this) {
        throw std::runtime_error(
            "Cannot add behavior to tower - behavior already belongs to "
            "another tower");
    }
    behavior->setTower(this);
    switch (behavior->getType()) {
        case BehaviorType::Combat:
            combatBehaviorPointer.reset(
                reinterpret_cast<CombatBehavior*>(behavior.release()));
            break;
        case BehaviorType::Resource:
            resourceBehaviorPointer.reset(
                reinterpret_cast<ResourceBehavior*>(behavior.release()));
            break;
        case BehaviorType::Glowing:
            glowingBehaviorPointer.reset(
                reinterpret_cast<GlowingBehavior*>(behavior.release()));
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

const EntityStat* Tower::getStats() const { return stats.get(); }

EntityStat* Tower::getStats() { return stats.get(); }

float Tower::getStat(const std::string& statName, float defaultValue) const {
    // Get base stat with any multipliers
    float baseStat = stats ? stats->getStatWithoutBonus(statName, defaultValue)
                           : defaultValue;

    // Add upgrade bonuses
    float upgradeBonus =
        upgradeManager ? upgradeManager->getTotalStatBonus(statName) : 0.0f;

    // Get multiplier
    float statMultiplier = 1.0f + stats->getStatWithoutBonus(
                                      EntityStat::multiplier(statName), 0.0f);

    return (baseStat + upgradeBonus) * statMultiplier;
}

float Tower::getBaseStat(const std::string& statName,
                         float defaultValue) const {
    return stats ? stats->getStatWithoutBonus(statName, defaultValue)
                 : defaultValue;
}

// Upgrade System Methods
UpgradeResult Tower::upgrade(int upgradeTypeId) {
    if (!upgradeManager) {
        return UpgradeResult::InvalidUpgradeType;
    }

    if (levelRef) {
        // levelRef->notify("sell_tower", this,
        // upgradeManager->getNextUpgradeDetail(upgradeTypeId)->cost);
        upgradeManager->upgrade(upgradeTypeId);
        return UpgradeResult::Success;
    }
    return UpgradeResult::InsufficientFunds;
}

bool Tower::canUpgrade(int upgradeTypeId,
                       const Currency& playerCurrency) const {
    return upgradeManager
               ? upgradeManager->canUpgrade(upgradeTypeId, playerCurrency)
               : false;
}

const UpgradeDetails* Tower::getNextUpgradeDetail(int upgradeTypeId) const {
    return upgradeManager ? upgradeManager->getNextUpgradeDetail(upgradeTypeId)
                          : nullptr;
}

int Tower::getUpgradeLevel(int upgradeTypeId) const {
    return upgradeManager ? upgradeManager->getCurrentLevel(upgradeTypeId) : 0;
}

std::vector<std::string> Tower::getAvailableEvolutions() const {
    return upgradeManager ? upgradeManager->getAvailableEvolutions()
                          : std::vector<std::string>{};
}

void Tower::setMaxTotalUpgrades(int maxUpgrades) {
    if (upgradeManager) {
        upgradeManager->setMaxTotalUpgrades(maxUpgrades);
    }
}

void Tower::addUpgradeType(int typeId,
                           std::unique_ptr<UpgradeType> upgradeType) {
    if (upgradeManager && upgradeType) {
        upgradeManager->addUpgradeType(typeId, std::move(upgradeType));
    }
}

void Tower::setStats(std::unique_ptr<EntityStat> newStats) {
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

void Tower::setRotation(const sf::Angle& rot) { setTurretRotation(rot); }

void Tower::setBaseRotation(const sf::Angle& rot) {
    baseRotation = rot;

    base.setRotation(rot);
}

void Tower::setTurretRotation(const sf::Angle& rot) {
    rotation = rot;  // Update Entity's rotation

    sprite.setRotation(rot);
}

void Tower::pointTurretTowards(const sf::Vector2f& targetPosition) {
    setTurretRotation((targetPosition - position).angle() + sf::degrees(90));
}

void Tower::loadBaseSpriteTexture(const sf::Texture& texture) {
    base = sf::Sprite(texture);

    sf::Vector2u originalSize = texture.getSize();

    sf::Vector2f origin(originalSize.x / 2.0f, originalSize.y / 2.0f);
    base.setOrigin(origin);
    base = Scaler::scaleSprite(base, {textureWidth, textureHeight});

    base.setPosition(position);
    base.setRotation(baseRotation);  // Use the stored base rotation
}

void Tower::loadTurretSpriteAnimation(
    const nlohmann::json& turretAnimationPath) {
    turretAnimation.loadJson(turretAnimationPath);
    turretAnimation.updateSpriteSize(turretWidth, turretHeight);
    turretAnimation.setCurrentFrame(turretAnimation.getFrameCount() - 1);

    updateSpriteTurretAnimation();
}

void Tower::updateSpriteTurretAnimation() {
    sprite = turretAnimation.getCurrentSprite();
    sprite.setPosition(position);
    sprite.setRotation(rotation);
}

void Tower::loadIcon() {
    if (iconRenderTexture.resize(static_cast<sf::Vector2u>(
            sf::Vector2f{textureWidth, textureHeight}))) {
        // Logger::success("Base plate resized successfully");
    } else {
        Logger::error("Failed to resize base plate");
    }
    iconRenderTexture.clear(sf::Color::Transparent);

    sf::Sprite baseClone = base;
    sf::Sprite turretClone = turretAnimation.getCurrentSprite();

    // Reset the scale to 1:1 for the icon
    // baseClone.setScale(sf::Vector2f(1.0f, 1.0f));
    // turretClone.setScale(sf::Vector2f(1.0f, 1.0f));
    baseClone = Scaler::scaleSprite(baseClone,
                                    {textureWidth / 2.f, textureHeight / 2.f});
    turretClone = Scaler::scaleSprite(turretClone, {textureWidth, textureHeight});

    // baseClone.setOrigin(baseClone.getLocalBounds().position +
    // baseClone.getLocalBounds().size / 2.f);

    // turretClone.setTextureRect({sf::Vector2i{0, 0}, {(int) textureWidth,
    // (int) textureHeight}});
    // turretClone.setOrigin(turretClone.getLocalBounds().position +
    // turretClone.getLocalBounds().size / 2.f);

    baseClone.setPosition(
        static_cast<sf::Vector2f>(iconRenderTexture.getSize()) / 2.f);
    turretClone.setPosition(
        static_cast<sf::Vector2f>(iconRenderTexture.getSize()) / 2.f);

    iconRenderTexture.draw(baseClone);
    iconRenderTexture.draw(turretClone);

    iconRenderTexture.display();
    icon = sf::Sprite(iconRenderTexture.getTexture());
}

void Tower::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(base, state);
    target.draw(sprite, state);
}

void Tower::updateCombatBehavior() {
    if (combatBehaviorPointer) {
        if (timer.isAvailable()) {
            std::vector<Enemy*> targets =
                levelRef->getEntityManager().getEnemies();
            if (targets.empty()) {
                return;
            }

            if (combatBehaviorPointer->engage(targets)) {
                turretAnimation.restart();
                turretAnimation.resume();
            }

            float fireRate = getStat(TowerStat::FIRE_RATE, 1.0f);
            float interval = (fireRate > 0.0f) ? (1.0f / fireRate) : 1.0f;
            timer.setTimeInterval(interval);
            timer.reset();
        }
    }
}

void Tower::update() {
    if (!levelRef) return;

    timer.update();
    turretAnimation.update();

    updateCombatBehavior();

    updateSpriteTurretAnimation();
}

bool Tower::contains(sf::Vector2f position) {
    return base.getGlobalBounds().contains(position);
}

bool Tower::intersects(sf::Vector2f points[4]) {
    sf::Rect bound = base.getGlobalBounds();
    sf::Vector2f baseBound[4] = {
        bound.position, bound.position + sf::Vector2f{bound.size.x, 0},
        bound.position + bound.size,
        bound.position + sf::Vector2f{0, bound.size.y}};
    return CollisionChecker::isQuadilateralCrossed(points, baseBound);
}

Tower::~Tower() = default;