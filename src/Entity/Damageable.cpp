#include "Entity/Damageable.hpp"

void Damageable::takeDamage(int damage) {
    if (health > 0) {
        health -= damage;
        if (health <= 0) {
            health = 0;
            onDeath();
        }
    }
}

void Damageable::heal(int healAmount) {
    health += healAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Damageable::setMaxHealth(int newMaxHealth) {
    maxHealth = newMaxHealth;
    if (health > maxHealth) {
        health = maxHealth;
    }
}