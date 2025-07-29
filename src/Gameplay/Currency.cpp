#include "Gameplay/Currency.hpp"

#include <algorithm>
#include <sstream>

#include "Core/ResourceManager.hpp"

// Static metadata initialization (will be loaded from JSON later)

// Constructors
Currency::Currency() : Currency(0, 0) {}

Currency::Currency(int scrapValue, int petroleumValue) {
    scrap.value = scrapValue;
    scrap.icon =
        sf::Sprite(*ResourceManager::getInstance().getTexture("scrap"));
    scrap.description =
        "Scrapped metal from the ruins of a once-vivid civilization";

    petroleum.value = petroleumValue;
    petroleum.icon =
        sf::Sprite(*ResourceManager::getInstance().getTexture("petroleum"));
    petroleum.description =
        "The blood of the industrial might. Useful for more advanced towers";
}

// Amount management
bool Currency::subtractScraps(int amount) {
    if (amount <= 0) return true;

    if (scrap.value >= amount) {
        scrap.value -= amount;
        return true;
    }
    return false;  // Not enough scrap
}

bool Currency::subtractPetroleum(int amount) {
    if (amount <= 0) return true;

    if (petroleum.value >= amount) {
        petroleum.value -= amount;
        return true;
    }
    return false;  // Not enough petroleum
}

bool Currency::canAfford(const Currency& cost) const {
    return hasEnoughScraps(cost.getScraps().value) &&
           hasEnoughPetroleum(cost.getPetroleum().value);
}

bool Currency::pay(const Currency& cost) {
    if (canAfford(cost)) {
        subtractScraps(cost.getScraps().value);
        subtractPetroleum(cost.getPetroleum().value);
        return true;  // Payment successful
    }
    return false;  // Not enough currency
}

// Arithmetic operators
Currency Currency::operator+(const Currency& other) const {
    return Currency(scrap.value + other.scrap.value,
                    petroleum.value + other.petroleum.value);
}

Currency Currency::operator-(const Currency& other) const {
    return Currency(std::max(0, scrap.value - other.scrap.value),
                    std::max(0, petroleum.value - other.petroleum.value));
}

Currency& Currency::operator+=(const Currency& other) {
    addScraps(other.scrap.value);
    addPetroleum(other.petroleum.value);
    return *this;
}

Currency& Currency::operator-=(const Currency& other) {
    subtractScraps(other.scrap.value);
    subtractPetroleum(other.petroleum.value);
    return *this;
}

// Comparison operators
bool Currency::operator==(const Currency& other) const {
    return scrap.value == other.scrap.value &&
           petroleum.value == other.petroleum.value;
}

bool Currency::operator!=(const Currency& other) const {
    return !(*this == other);
}

// Scalar multiplication
Currency Currency::operator*(int multiplier) const {
    int safeMultiplier = std::max(0, multiplier);  // Ensure non-negative
    return Currency(scrap.value * safeMultiplier,
                    petroleum.value * safeMultiplier);
}

Currency& Currency::operator*=(int multiplier) {
    int safeMultiplier = std::max(0, multiplier);  // Ensure non-negative
    scrap.value *= safeMultiplier;
    petroleum.value *= safeMultiplier;
    return *this;
}

// Utility methods
bool Currency::isEmpty() const {
    return scrap.value == 0 && petroleum.value == 0;
}

void Currency::clear() {
    scrap.value = 0;
    petroleum.value = 0;
}

// Global operators
Currency operator*(int multiplier, const Currency& currency) {
    return currency * multiplier;
}
