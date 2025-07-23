#include "EntityManager.hpp"

#include "Core/MouseState.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/EnemyPanel.hpp"
#include "Core/KeyboardState.hpp"
void EntityManager::update() {
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

void EntityManager::render(sf::RenderStates state) const {
    // Render towers
    for (const auto& tower : towers) {
        if (tower) {
            Window::getInstance().draw(*tower, state);
        }
    }

    // Render enemies
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            Window::getInstance().draw(*enemy, state);
        }
    }

    // Render projectiles
    for (const auto& projectile : projectiles) {
        if (projectile && projectile->isAlive()) {
            Window::getInstance().draw(*projectile, state);
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

    // Remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const std::unique_ptr<Enemy>& enemy) {
                                     return !enemy || !enemy->isAlive();
                                 }),
                  enemies.end());

    // Remove dead projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const std::unique_ptr<Projectile>& projectile) {
                           return !projectile || !projectile->isAlive();
                       }),
        projectiles.end());
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

void EntityManager::onMouseEvent(Mouse button, UserEvent event,
                                 const sf::Vector2f& worldPosition,
                                 const sf::Vector2f& windowPosition) {
    if (!(button == Mouse::Left && event == UserEvent::Press)) return;

    Enemy* foundEnemy = nullptr;
    for (auto& enemy : enemies) {
        if (enemy->contains(worldPosition)) {
            foundEnemy = enemy.get();
            break;
        }
    }
    if (foundEnemy)
        EnemyPanel::getInstance().setEnemy(*foundEnemy);
    else
        EnemyPanel::getInstance().clearEnemy();
}

void EntityManager::onKeyEvent(Key key, UserEvent event,
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
        if (foundEnemy) foundEnemy->onHit(50);
    }
    if (key == Key::F && event == UserEvent::Press) {
        Enemy* foundEnemy = nullptr;
        for (auto& enemy : enemies) {
            if (enemy->contains(worldPosition)) {
                foundEnemy = enemy.get();
                break;
            }
        }
        if (foundEnemy) foundEnemy->onHeal(50);
    }
};