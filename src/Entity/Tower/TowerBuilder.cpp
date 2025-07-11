#include "Entity/Tower/TowerBuilder.hpp"
#include "Scene/Scene.hpp"
#include <stdexcept>
#include <iostream>

TowerBuilder::TowerBuilder() 
    : scene(nullptr), buildable(true), cost(0, 0), 
      position(0, 0), angle(sf::radians(0.f)), 
      textureWidth(32.0f), textureHeight(32.0f),
      timerInterval(1.0f), isBuilt(false) {
}

TowerBuilder& TowerBuilder::reset() {
    id.clear();
    scene = nullptr;
    name.clear();
    description.clear();
    buildable = true;
    cost = Currency(0, 0);
    position = sf::Vector2f(0, 0);
    angle = sf::radians(0.f);
    baseTexturePath.clear();
    turretTexturePath.clear();
    textureWidth = 32.0f;
    textureHeight = 32.0f;
    stats.reset();
    behaviors.clear();
    timerInterval = 1.0f;
    isBuilt = false;
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
    this->baseTexturePath = texturePath;
    return *this;
}

TowerBuilder& TowerBuilder::setTurretTexturePath(const std::string& texturePath) {
    this->turretTexturePath = texturePath;
    return *this;
}

TowerBuilder& TowerBuilder::setStats(std::unique_ptr<TowerStat> stats) {
    this->stats = std::move(stats);
    return *this;
}

TowerBuilder& TowerBuilder::addBehavior(std::unique_ptr<TowerBehavior> behavior) {
    if (behavior) {
        behaviors.push_back(std::move(behavior));
    }
    return *this;
}

TowerBuilder& TowerBuilder::setTimerInterval(float interval) {
    this->timerInterval = interval;
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
        throw std::invalid_argument("TowerBuilder: Cannot build multiple times. Call reset() first.");
    }
    
    validate();
    
    // Create the tower with required parameters
    auto tower = std::make_unique<Tower>(*scene, id, position, angle);
    
    // Configure the tower with optional parameters
    tower->setName(name);
    tower->setDescription(description);
    tower->setBuildable(buildable);
    tower->setCost(cost);
    
    // Set timer interval
    tower->setTimerInterval(timerInterval);
    
    // Set texture dimensions
    tower->setTextureDimensions(textureWidth, textureHeight);
    
    // Set statistics if provided
    if (stats) {
        tower->setStats(std::move(stats));
    }
    
    // Add behaviors
    for (auto& behavior : behaviors) {
        tower->addBehavior(std::move(behavior));
    }
    behaviors.clear(); // Clear since we moved them
    
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
        throw std::invalid_argument("TowerBuilder: Scene reference is required.");
    }
    
    // Additional validation could be added here
    if (cost.getScraps() < 0 || cost.getPetroleum() < 0) {
        throw std::invalid_argument("TowerBuilder: Cost cannot be negative.");
    }
}

void TowerBuilder::loadTextures(Tower& tower) const {
    // Load textures using the new Scene texture methods
    
    if (!baseTexturePath.empty()) {
        std::cout << "TowerBuilder: Loading base texture from: " << baseTexturePath << std::endl;
        
        try {
            // Load texture into ResourceManager through Scene
            std::string baseTextureId = "tower_base_" + id;
            scene->loadTexture(baseTexturePath, baseTextureId);
            
            const sf::Texture* baseTexture = scene->getTexture(baseTextureId);
            if (baseTexture) {
                tower.loadBaseSpriteTexture(*baseTexture);
                std::cout << "TowerBuilder: Successfully loaded base texture" << std::endl;
            } else {
                std::cout << "TowerBuilder: Failed to get base texture from ResourceManager" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "TowerBuilder: Error loading base texture: " << e.what() << std::endl;
        }
    }
    
    if (!turretTexturePath.empty()) {
        std::cout << "TowerBuilder: Loading turret texture from: " << turretTexturePath << std::endl;
        
        try {
            // Load texture into ResourceManager through Scene
            std::string turretTextureId = "tower_turret_" + id;
            scene->loadTexture(turretTexturePath, turretTextureId);
            
            const sf::Texture* turretTexture = scene->getTexture(turretTextureId);
            if (turretTexture) {
                tower.loadTurretSpriteTexture(*turretTexture);
                std::cout << "TowerBuilder: Successfully loaded turret texture" << std::endl;
            } else {
                std::cout << "TowerBuilder: Failed to get turret texture from ResourceManager" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "TowerBuilder: Error loading turret texture: " << e.what() << std::endl;
        }
    }
}
