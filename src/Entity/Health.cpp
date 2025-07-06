#include "Entity/Health.hpp"

void Health::clampHealth() {
    if (currentHealth < 0) currentHealth = 0;
    if (currentHealth > maxHealth) currentHealth = maxHealth;
}
void Health::takeDamage(float amount) {
    currentHealth -= amount;
    clampHealth();
}

void Health::heal(float amount) {
    currentHealth += amount;
    clampHealth();
}

float Health::getHealthToMaxHealthRatio() { return currentHealth / maxHealth; }

void Health::setHealth(float amount) {
    currentHealth = amount;
    clampHealth();
}

void Health::setMaxHealth(float amount) {
    maxHealth = amount;
    if (maxHealth < 0) maxHealth = 0;
}

float Health::getMaxHealth() { return maxHealth; }
float Health::getHealth() { return currentHealth; }