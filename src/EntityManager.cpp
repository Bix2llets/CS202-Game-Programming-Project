#include "EntityManager.hpp"

#include "Core/KeyboardState.hpp"
#include "Core/MouseState.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/EnemyPanel.hpp"
#include "GUIComponents/cursor.hpp"
#include "Scene/Level.hpp"

void EntityManager::update() {
    // Update towers
    for (auto& tower : towers) {
        if (tower) {
            tower->update();
        }
    }

    // Update enemies
    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            enemy->update();
        }
    }

    // Update projectiles
    for (auto& projectile : projectiles) {
        if (projectile && projectile->isFlying()) {
            projectile->update();
        }
    }

    // Update area effects
    for (auto& effect : areaEffects) {
        if (effect && effect->isActive()) {
            effect->update();
        }
    }

    // Clean up dead entities
    cleanup();
}

void EntityManager::render(sf::RenderStates state) const {
    // Render static entities
    for (const auto& entity : staticEntities) {
        if (entity) {
            Window::getInstance().getRenderWindow().draw(*entity, state);
        }
    }

    // Render towers
    for (const auto& tower : towers) {
        if (tower) {
            Window::getInstance().getRenderWindow().draw(*tower, state);
        }
    }

    // Render area effects (draw below enemies for ground feel)
    for (const auto& effect : areaEffects) {
        if (effect) {
            Window::getInstance().getRenderWindow().draw(*effect, state);
        }
    }

    // Render enemies
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            Window::getInstance().getRenderWindow().draw(*enemy, state);
        }
    }

    // Render projectiles
    for (const auto& projectile : projectiles) {
        if (projectile) {
            Window::getInstance().getRenderWindow().draw(*projectile, state);
        }
    }
}

void EntityManager::cleanup() {
    // Remove dead towers
    towers.erase(std::remove_if(towers.begin(), towers.end(),
                                [](const std::unique_ptr<Tower>& tower) {
                                    return !tower;
                                }),
                 towers.end());

    staticEntities.erase(
        std::remove_if(staticEntities.begin(), staticEntities.end(),
                       [](const std::unique_ptr<StaticEntity>& entity) {
                           return !entity;
                       }),
        staticEntities.end());

    // Remove dead enemies

    auto enemyCleanup = [this](const std::unique_ptr<Enemy>& enemy) {
        if (!enemy || !enemy->isAlive()) {
            if (enemy && !enemy->isFinished()) {
                level.notify("add_currency", *enemy,
                             Currency(enemy->getScrapReward(),
                                      enemy->getPetroleumReward()));
                return true;  // Remove this enemy
            }
            if (enemy && enemy->isFinished()) {
                level.notify("enemy_passed", enemy.get());
            }
            return true;
        }
        return false;  // Keep this enemy
    };
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), enemyCleanup),
                  enemies.end());

    // Remove dead projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const std::unique_ptr<Projectile>& projectile) {
                           return !projectile || !projectile->isFlying();
                       }),
        projectiles.end());

    // Remove expired area effects
    areaEffects.erase(
        std::remove_if(areaEffects.begin(), areaEffects.end(),
                       [](const std::unique_ptr<AreaEffect>& effect) {
                           return !effect || !effect->isActive();
                       }),
        areaEffects.end());
}

void EntityManager::addTower(std::unique_ptr<Tower> tower) {
    if (tower) {
        towers.push_back(std::move(tower));
    }
}

void EntityManager::addEnemy(std::unique_ptr<Enemy> enemy) {
    if (enemy) {
        enemies.push_back(std::move(enemy));
    }
}

void EntityManager::addProjectile(std::unique_ptr<Projectile> projectile) {
    if (projectile) {
        projectiles.push_back(std::move(projectile));
    }
}

void EntityManager::addAreaEffect(std::unique_ptr<AreaEffect> effect) {
    if (effect) {
        areaEffects.push_back(std::move(effect));
    }
}

void EntityManager::addStaticEntity(std::unique_ptr<StaticEntity> staticEntity) {
    if (staticEntity) {
        staticEntities.push_back(std::move(staticEntity));
    }
}

std::vector<Enemy*> EntityManager::getEnemies() {
    std::vector<Enemy*> enemyPtrs;
    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            enemyPtrs.push_back(enemy.get());
        }
    }
    return enemyPtrs;
}

std::vector<Tower*> EntityManager::getTowers() {
    std::vector<Tower*> towerPtrs;
    for (auto& tower : towers) {
        if (tower) {
            towerPtrs.push_back(tower.get());
        }
    }
    return towerPtrs;
}

std::vector<AreaEffect*> EntityManager::getAreaEffects() {
    std::vector<AreaEffect*> effectPtrs;
    for (auto& e : areaEffects) {
        if (e && e->isActive()) {
            effectPtrs.push_back(e.get());
        }
    }
    return effectPtrs;
}

std::vector<StaticEntity*> EntityManager::getStaticEntities() {
    std::vector<StaticEntity*> staticEntityPtrs;
    for (auto& staticEntity : staticEntities) {
        if (staticEntity) {
            staticEntityPtrs.push_back(staticEntity.get());
        }
    }
    return staticEntityPtrs;
}

void EntityManager::clear() {
    towers.clear();
    enemies.clear();
    projectiles.clear();
    areaEffects.clear();
}

size_t EntityManager::getTotalEntityCount() const {
    return towers.size() + enemies.size() + projectiles.size() +
           areaEffects.size();
}

bool EntityManager::onMouseEvent(Mouse button, UserEvent event,
                                 const sf::Vector2f& worldPosition,
                                 const sf::Vector2f& windowPosition) {
    if ((button == Mouse::Left && event == UserEvent::Press)) {
        Enemy* foundEnemy = nullptr;
        for (auto& enemy : enemies) {
            if (enemy->contains(worldPosition)) {
                foundEnemy = enemy.get();
                break;
            }
        }
        if (foundEnemy) {
            EnemyPanel::getInstance().setEnemy(*foundEnemy);
            return true;
        }
        EnemyPanel::getInstance().clearEnemy();

        Tower* foundTower = nullptr;
        for (auto& tower : towers) {
            if (tower->contains(worldPosition)) {
                foundTower = tower.get();
                break;
            }
        }

        if (foundTower) {
            level.notify("focus_tower", foundTower);
            return true;
        }
        level.notify("unfocus_tower", nullptr);

        return false;
    }

    if (event == UserEvent::Move) {
    }
    return false;
}

bool EntityManager::onKeyEvent(Key key, UserEvent event,
                               const sf::Vector2f& worldPosition,
                               const sf::Vector2f& windowPosition) {
    if (key == Key::D && event == UserEvent::Press) {
        Enemy* foundEnemy = nullptr;
        for (auto& enemy : enemies) {
            if (enemy->contains(worldPosition)) {
                foundEnemy = enemy.get();
                break;
            }
        }
        if (foundEnemy) {
            foundEnemy->onHit(50);
            return true;
        }
    }
    if (key == Key::F && event == UserEvent::Press) {
        Enemy* foundEnemy = nullptr;
        for (auto& enemy : enemies) {
            if (enemy->contains(worldPosition)) {
                foundEnemy = enemy.get();
                break;
            }
        }
        if (foundEnemy) {
            foundEnemy->onHeal(50);
            return true;
        }
        return false;
    }
    return false;
};

bool EntityManager::onScrollEvent(float delta,
                                  const sf::Vector2f& worldPosition,
                                  const sf::Vector2f& windowPosition) {
    return false;
}

void EntityManager::removeTower(Tower* tower) {
    if (!tower) return;
    for (auto& x : towers) {
        if (x.get() == tower) {
            x.reset();
            break;
        }
    }
    cleanup();
}