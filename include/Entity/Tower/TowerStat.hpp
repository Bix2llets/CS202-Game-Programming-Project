#pragma once

class Level;
#include "Entity/Cooldown.hpp"
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
class Level;
class TowerStat {
    public:
    virtual void setLevel(Level level) = 0;
    virtual std::unordered_map<std::string, float> getStat() = 0;
    virtual void upgradePath1(int level) = 0;
    virtual void upgradePath2(int level) = 0;
    virtual void upgradePath3(int level) = 0;
    bool canUpgrade(int path);
    virtual sf::Sprite getUpgradeIcons(int level) = 0;
};