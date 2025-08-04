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
            Window::getInstance().getRenderWindow().draw(*tower, state);
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
        if (projectile && projectile->isAlive()) {
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

    // Remove dead enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [this](const std::unique_ptr<Enemy>& enemy) {
                           if (!enemy) return true;
                           if (!enemy->isAlive()) {
                               Logger::debug(std::format(
                                   "Reward: {} {}", enemy->getPetroleumReward(),
                                   enemy->getScrapReward()));
                               level.notify("add_currency", *enemy,
                                            Currency(enemy->getScrapReward(), enemy->getPetroleumReward()));
                               return true;
                           }
                           return false;
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