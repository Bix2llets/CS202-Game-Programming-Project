#include "Entity/Tower/TowerBuilder.hpp"

#include <iostream>
#include <stdexcept>

#include "Core/ResourceManager.hpp"
#include "Entity/Tower/Upgrades/UpgradeType.hpp"
#include "Scene/Scene.hpp"

TowerBuilder::TowerBuilder()
    : scene(nullptr),
      buildable(true),
      cost(0, 0),
      position(0, 0),
      angle(sf::radians(0.f)),
      textureWidth(32.0f),
      textureHeight(32.0f),
      timerInterval(1.0f),
      isBuilt(false),
      maxTotalUpgrades(10) {}

TowerBuilder& TowerBuilder::reset() {
    id.clear();
    scene = nullptr;
    name.clear();
    description.clear();
    buildable = true;
    cost = Currency(0, 0);
    position = sf::Vector2f(0, 0);
    angle = sf::radians(0.f);
    baseTextureId.clear();
    turretAnimationPath.clear();
    textureWidth = 32.0f;
    textureHeight = 32.0f;
    stats.reset();
    behaviors.clear();
    timerInterval = 1.0f;
    isBuilt = false;
    maxTotalUpgrades = 10;
    upgradeTypes.clear();
    return *this;
}

TowerBuilder& TowerBuilder::setId(const std::string& id) {
    this->id = id;
    return *this;
}

TowerBuilder& TowerBuilder::setScene(Scene& scene) {
    this->scene = &scene;
    return *this;
}

TowerBuilder& TowerBuilder::setName(const std::string& name) {
    this->name = name;
    return *this;
}

TowerBuilder& TowerBuilder::setDescription(const std::string& description) {
    this->description = description;
    return *this;
}

TowerBuilder& TowerBuilder::setBuildable(bool buildable) {
    this->buildable = buildable;
    return *this;
}

TowerBuilder& TowerBuilder::setCost(const Currency& cost) {
    this->cost = cost;
    return *this;
}

TowerBuilder& TowerBuilder::setPosition(const sf::Vector2f& position) {
    this->position = position;
    return *this;
}

TowerBuilder& TowerBuilder::setAngle(const sf::Angle& angle) {
    this->angle = angle;
    return *this;
}

TowerBuilder& TowerBuilder::setBaseTexturePath(const std::string& texturePath) {
    this->baseTextureId = texturePath;
    return *this;
}

TowerBuilder& TowerBuilder::setTurretAnimationPath(const nlohmann::json& jsonFile) {
    this->turretAnimationPath = jsonFile;
    return *this;
}

TowerBuilder& TowerBuilder::setStats(std::unique_ptr<TowerStat> stats) {
    this->stats = std::move(stats);
    return *this;
}

TowerBuilder& TowerBuilder::addBehavior(
    std::unique_ptr<TowerBehavior> behavior) {
    if (behavior) {
        behaviors.push_back(std::move(behavior));
    }
    return *this;
}

TowerBuilder& TowerBuilder::setTimerInterval(float interval) {
    this->timerInterval = interval;
    return *this;
}

TowerBuilder& TowerBuilder::setMaxTotalUpgrades(int maxUpgrades) {
    this->maxTotalUpgrades = maxUpgrades;
    return *this;
}

TowerBuilder& TowerBuilder::addUpgradeType(
    int typeId, std::unique_ptr<UpgradeType> upgradeType) {
    if (upgradeType) {
        upgradeTypes[typeId] = std::move(upgradeType);
    }
    return *this;
}

TowerBuilder& TowerBuilder::addSimpleUpgradeType(
    int typeId, const std::string& displayName, const std::string& description,
    int maxLevel, const std::string& iconID, const std::string& evolveTo) {
    auto upgradeType = std::make_unique<UpgradeType>(
        displayName, description, maxLevel, iconID, evolveTo);
    upgradeTypes[typeId] = std::move(upgradeType);
    return *this;
}

TowerBuilder& TowerBuilder::setTextureWidth(float width) {
    this->textureWidth = width;
    return *this;
}

TowerBuilder& TowerBuilder::setTextureHeight(float height) {
    this->textureHeight = height;
    return *this;
}

TowerBuilder& TowerBuilder::setTextureDimensions(float width, float height) {
    this->textureWidth = width;
    this->textureHeight = height;
    return *this;
}

std::unique_ptr<Tower> TowerBuilder::build() {
    if (isBuilt) {
        throw std::invalid_argument(
            "TowerBuilder: Cannot build multiple times. Call reset() first.");
    }

    validate();

    // Create the tower with required parameters
    auto tower = std::make_unique<Tower>(*scene, id, position, angle);

    // Configure the tower with optional parameters
    tower->name = name;
    tower->description = description;
    tower->buildable = buildable;
    tower->cost = cost;
    tower->totalCost = cost; 

    // Set timer interval
    tower->timer.setTimeInterval(timerInterval)
        .setRemainingTime(0)
        .setTimerMode(TimerMode::Single);

    // Set texture dimensions
    tower->textureWidth = textureWidth;
    tower->textureHeight = textureHeight;

    // Set statistics if provided
    if (stats) {
        tower->setStats(std::move(stats));
    }

    // Add behaviors
    for (auto& behavior : behaviors) {
        tower->addBehavior(std::move(behavior));
    }
    behaviors.clear();  // Clear since we moved them

    // Configure upgrade system
    tower->setMaxTotalUpgrades(maxTotalUpgrades);
    for (auto& [typeId, upgradeType] : upgradeTypes) {
        // Move the upgrade type to the tower
        tower->addUpgradeType(typeId, std::move(upgradeType));
    }
    upgradeTypes.clear();  // Clear since we moved them

    // Load textures
    loadTextures(*tower);

    isBuilt = true;
    return tower;
}

void TowerBuilder::validate() const {
    if (id.empty()) {
        throw std::invalid_argument("TowerBuilder: Tower ID is required.");
    }

    if (scene == nullptr) {
        throw std::invalid_argument(
            "TowerBuilder: Scene reference is required.");
    }

    // Validate cost only for buildable towers
    if (buildable) {
        if (cost.getScraps().value < 0 || cost.getPetroleum().value < 0) {
            throw std::invalid_argument(
                "TowerBuilder: Cost cannot be negative for buildable towers.");
        }
    }

    // Validate timer interval
    if (timerInterval <= 0.0f) {
        throw std::invalid_argument(
            "TowerBuilder: Timer interval must be positive.");
    }

    // Validate texture dimensions
    if (textureWidth <= 0.0f || textureHeight <= 0.0f) {
        throw std::invalid_argument(
            "TowerBuilder: Texture dimensions must be positive.");
    }

    // Validate upgrade system consistency
    if (maxTotalUpgrades < 0) {
        throw std::invalid_argument(
            "TowerBuilder: Maximum total upgrades cannot be negative.");
    }

    // Validate upgrade types (only if there are any)
    if (!upgradeTypes.empty()) {
        for (const auto& [typeId, upgradeType] : upgradeTypes) {
            if (!upgradeType) {
                throw std::invalid_argument("TowerBuilder: Upgrade type " +
                                            std::to_string(typeId) +
                                            " is null.");
            }

            if (upgradeType->getMaxLevel() <= 0) {
                throw std::invalid_argument("TowerBuilder: Upgrade type " +
                                            std::to_string(typeId) +
                                            " must have positive max level.");
            }

            if (upgradeType->getDisplayName().empty()) {
                throw std::invalid_argument("TowerBuilder: Upgrade type " +
                                            std::to_string(typeId) +
                                            " must have a display name.");
            }
        }

        // Calculate total possible upgrades
        int totalPossibleUpgrades = 0;
        for (const auto& [typeId, upgradeType] : upgradeTypes) {
            totalPossibleUpgrades += upgradeType->getMaxLevel();
        }

        // Warn if max total upgrades is higher than what's possible (not an
        // error, just a warning)
        if (maxTotalUpgrades > totalPossibleUpgrades) {
            std::cout << "TowerBuilder Warning: Max total upgrades ("
                      << maxTotalUpgrades
                      << ") is higher than total possible upgrades ("
                      << totalPossibleUpgrades << ")." << std::endl;
        }
    }

    // Validate that non-buildable towers don't have upgrade types (per your
    // requirements)
    if (!buildable && !upgradeTypes.empty()) {
        throw std::invalid_argument(
            "TowerBuilder: Non-buildable towers should not have upgrade types "
            "defined. They inherit upgrades during evolution.");
    }
}

void TowerBuilder::loadTextures(Tower& tower) const {
    // Load textures using the new Scene texture methods
    if (!baseTextureId.empty()) {
        std::cout << "TowerBuilder: Loading base texture from: "
                  << baseTextureId << std::endl;

        try {
            const sf::Texture* baseTexture = ResourceManager::getInstance().getTexture(baseTextureId);
            if (baseTexture) {
                tower.loadBaseSpriteTexture(*baseTexture);
                std::cout << "TowerBuilder: Successfully loaded base texture"
                          << std::endl;
            } else {
                std::cout << "TowerBuilder: Failed to get base texture from "
                             "ResourceManager"
                          << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "TowerBuilder: Error loading base texture: "
                      << e.what() << std::endl;
        }
    }

    if(turretAnimationPath != nullptr) {
        std::cout << "TowerBuilder: Loading turret animation from: "
                  << turretAnimationPath.dump() << std::endl;

        try {
            tower.loadTurretSpriteAnimation(turretAnimationPath);
            std::cout << "TowerBuilder: Successfully loaded turret animation"
                      << std::endl;
        } catch (const std::exception& e) {
            std::cout << "TowerBuilder: Error loading turret animation: "
                      << e.what() << std::endl;
        }
    }
    
    // Load the icon after both base and turret textures are loaded
    std::cout << "TowerBuilder: Loading combined icon sprite" << std::endl;
    try {
        tower.loadIcon();
        std::cout << "TowerBuilder: Successfully loaded icon sprite" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "TowerBuilder: Error loading icon sprite: " << e.what() << std::endl;
    }
}
