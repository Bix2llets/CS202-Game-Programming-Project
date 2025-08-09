#include "Entity/Health.hpp"

void Health::clampHealth() {
    if (currentHealth < 0) currentHealth = 0;
    if (currentHealth > maxHealth) currentHealth = maxHealth;
}
Health& Health::takeDamage(float amount) {
    currentHealth -= amount;
    clampHealth();
    return *this;
}

Health& Health::heal(float amount) {
    currentHealth += amount;
    clampHealth();
    return *this;
}

float Health::getHealthToMaxHealthRatio() { return currentHealth / maxHealth; }

Health& Health::setHealth(float amount) {
    currentHealth = amount;
    clampHealth();
    return *this;
}

Health& Health::setMaxHealth(float amount) {
    maxHealth = amount;
    if (maxHealth < 0) maxHealth = 0;
    return *this;
}

float Health::getMaxHealth() const { return maxHealth; }
float Health::getHealth() const { return currentHealth; }