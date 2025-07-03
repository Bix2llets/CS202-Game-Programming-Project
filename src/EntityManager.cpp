#include "EntityManager.hpp"

void EntityManager::updateAll() {
    // Update towers
    for (auto& tower : towers) {
        if (tower) {
            tower->update();
            
            // Make towers attack enemies
            auto enemyPtrs = getEnemies();
            // tower->attack(enemyPtrs);
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
        if (projectile && projectile->isAlive()) {
            projectile->update();
        }
    }

    // Clean up dead entities
    cleanup();
}

void EntityManager::renderAll() {
    // Render towers
    for (const auto& tower : towers) {
        if (tower) {
            window.draw(*tower);
        }
    }

    // Render enemies
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            window.draw(*enemy);
        }
    }

    // Render projectiles
    for (const auto& projectile : projectiles) {
        if (projectile && projectile->isAlive()) {
            window.draw(*projectile);
        }
    }
}

void EntityManager::cleanup() {
    // Remove dead towers
    towers.erase(
        std::remove_if(towers.begin(), towers.end(),
            [](const std::unique_ptr<Tower>& tower) {
                return !tower;
            }),
        towers.end()
    );

    // Remove dead enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return !enemy || !enemy->isAlive();
            }),
        enemies.end()
    );

    // Remove dead projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const std::unique_ptr<Projectile>& projectile) {
                return !projectile || !projectile->isAlive();
            }),
        projectiles.end()
    );

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

void EntityManager::clear() {
    towers.clear();
    enemies.clear();
    projectiles.clear();
}

size_t EntityManager::getTotalEntityCount() const {
    return towers.size() + enemies.size() + projectiles.size();
}
