#include "Gameplay/Currency.hpp"

#include <algorithm>
#include <sstream>

// Static metadata initialization (will be loaded from JSON later)
sf::Texture Currency::scrapsIcon;
sf::Texture Currency::petroleumIcon;
std::string Currency::scrapsDescription;
std::string Currency::petroleumDescription;

// Constructors
Currency::Currency() : scraps(0), petroleum(0) {}

Currency::Currency(int scraps, int petroleum) 
    : scraps(std::max(0, scraps)), petroleum(std::max(0, petroleum)) {}

// Static metadata management
void Currency::loadMetadata() {
    // TODO: Implement JSON loading for icons and descriptions
    // This will be implemented when JSON loading system is added
}

// Amount management
bool Currency::subtractScraps(int amount) {
    if (amount <= 0) return true;
    
    if (scraps >= amount) {
        scraps -= amount;
        return true;
    }
    return false; // Not enough scraps
}

bool Currency::subtractPetroleum(int amount) {
    if (amount <= 0) return true;
    
    if (petroleum >= amount) {
        petroleum -= amount;
        return true;
    }
    return false; // Not enough petroleum
}

bool Currency::canAfford(const Currency& cost) const {
    return hasEnoughScraps(cost.getScraps()) && hasEnoughPetroleum(cost.getPetroleum());
}

bool Currency::pay(const Currency& cost) {
    if (canAfford(cost)) {
        subtractScraps(cost.getScraps());
        subtractPetroleum(cost.getPetroleum());
        return true; // Payment successful
    }
    return false; // Not enough currency
}

// Arithmetic operators
Currency Currency::operator+(const Currency& other) const {
    return Currency(scraps + other.scraps, petroleum + other.petroleum);
}

Currency Currency::operator-(const Currency& other) const {
    return Currency(
        std::max(0, scraps - other.scraps),
        std::max(0, petroleum - other.petroleum)
    );
}

Currency& Currency::operator+=(const Currency& other) {
    addScraps(other.scraps);
    addPetroleum(other.petroleum);
    return *this;
}

Currency& Currency::operator-=(const Currency& other) {
    subtractScraps(other.scraps);
    subtractPetroleum(other.petroleum);
    return *this;
}

// Comparison operators
bool Currency::operator==(const Currency& other) const {
    return scraps == other.scraps && petroleum == other.petroleum;
}

bool Currency::operator!=(const Currency& other) const {
    return !(*this == other);
}

bool Currency::operator<(const Currency& other) const {
    return scraps < other.scraps && petroleum < other.petroleum;
}

bool Currency::operator<=(const Currency& other) const {
    return scraps <= other.scraps && petroleum <= other.petroleum;
}

bool Currency::operator>(const Currency& other) const {
    return scraps > other.scraps && petroleum > other.petroleum;
}

bool Currency::operator>=(const Currency& other) const {
    return scraps >= other.scraps && petroleum >= other.petroleum;
}

// Scalar multiplication
Currency Currency::operator*(int multiplier) const {
    int safeMultiplier = std::max(0, multiplier); // Ensure non-negative
    return Currency(scraps * safeMultiplier, petroleum * safeMultiplier);
}

Currency& Currency::operator*=(int multiplier) {
    int safeMultiplier = std::max(0, multiplier); // Ensure non-negative
    scraps *= safeMultiplier;
    petroleum *= safeMultiplier;
    return *this;
}

// Utility methods
std::string Currency::toString() const {
    std::ostringstream oss;
    oss << "Currency{Scraps: " << scraps << ", Petroleum: " << petroleum << "}";
    return oss.str();
}

bool Currency::isEmpty() const {
    return scraps == 0 && petroleum == 0;
}

void Currency::clear() {
    scraps = 0;
    petroleum = 0;
}

// Global operators
Currency operator*(int multiplier, const Currency& currency) {
    return currency * multiplier;
}
