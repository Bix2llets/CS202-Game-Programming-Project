/**
 * @file Currency.hpp
 * @brief Declares the Currency class for managing different types of in-game currency.
 *
 * The Currency class handles two types of currency: Scraps and Petroleum.
 * Icons and descriptions are loaded from JSON files.
 */
#pragma once

#include <string>
#include <SFML/Graphics/Texture.hpp>

/**
 * @class Currency
 * @brief Manages two types of in-game currency: Scraps and Petroleum.
 *
 * Simple currency management with direct member variables for better performance.
 * Metadata (icons, descriptions) will be loaded from JSON configuration files.
 */
class Currency {
private:
    int scraps;     ///< Amount of scraps currency
    int petroleum;  ///< Amount of petroleum currency

    // Static metadata (to be loaded from JSON later)
    static sf::Texture scrapsIcon;      ///< Icon texture for scraps
    static sf::Texture petroleumIcon;   ///< Icon texture for petroleum
    static std::string scrapsDescription;    ///< Description for scraps
    static std::string petroleumDescription; ///< Description for petroleum

public:
    /**
     * @brief Default constructor - initializes all currencies to 0.
     */
    Currency();

    /**
     * @brief Constructor with initial amounts.
     * @param scraps Initial amount of scraps.
     * @param petroleum Initial amount of petroleum.
     */
    Currency(int scraps, int petroleum);

    /**
     * @brief Copy constructor.
     * @param other Currency object to copy from.
     */
    Currency(const Currency& other) = default;

    /**
     * @brief Assignment operator.
     * @param other Currency object to assign from.
     * @return Reference to this Currency object.
     */
    Currency& operator=(const Currency& other) = default;

    /**
     * @brief Destructor.
     */
    ~Currency() = default;

    // Static methods for metadata access (to be loaded from JSON)
    /**
     * @brief Load currency metadata from JSON files.
     * This will be implemented when JSON loading is added.
     */
    static void loadMetadata();

    /**
     * @brief Get the icon texture for scraps.
     * @return const sf::Texture& Scraps icon texture.
     */
    static const sf::Texture& getScrapsIcon() { return scrapsIcon; }

    /**
     * @brief Get the icon texture for petroleum.
     * @return const sf::Texture& Petroleum icon texture.
     */
    static const sf::Texture& getPetroleumIcon() { return petroleumIcon; }

    /**
     * @brief Get the description for scraps.
     * @return const std::string& Scraps description.
     */
    static const std::string& getScrapsDescription() { return scrapsDescription; }

    /**
     * @brief Get the description for petroleum.
     * @return const std::string& Petroleum description.
     */
    static const std::string& getPetroleumDescription() { return petroleumDescription; }

    // Amount management
    /**
     * @brief Get the current amount of scraps.
     * @return int Current scraps amount.
     */
    int getScraps() const { return scraps; }

    /**
     * @brief Get the current amount of petroleum.
     * @return int Current petroleum amount.
     */
    int getPetroleum() const { return petroleum; }

    /**
     * @brief Set the amount of scraps.
     * @param amount New scraps amount.
     */
    void setScraps(int amount) { scraps = std::max(0, amount); }

    /**
     * @brief Set the amount of petroleum.
     * @param amount New petroleum amount.
     */
    void setPetroleum(int amount) { petroleum = std::max(0, amount); }

    /**
     * @brief Add to the scraps amount.
     * @param amount Amount to add.
     */
    void addScraps(int amount) { if (amount > 0) scraps += amount; }

    /**
     * @brief Add to the petroleum amount.
     * @param amount Amount to add.
     */
    void addPetroleum(int amount) { if (amount > 0) petroleum += amount; }

    /**
     * @brief Subtract from the scraps amount.
     * @param amount Amount to subtract.
     * @return bool True if subtraction was successful (had enough currency), false otherwise.
     */
    bool subtractScraps(int amount);

    /**
     * @brief Subtract from the petroleum amount.
     * @param amount Amount to subtract.
     * @return bool True if subtraction was successful (had enough currency), false otherwise.
     */
    bool subtractPetroleum(int amount);

    /**
     * @brief Check if there's enough scraps.
     * @param amount Amount to check for.
     * @return bool True if there's enough scraps, false otherwise.
     */
    bool hasEnoughScraps(int amount) const { return scraps >= amount; }

    /**
     * @brief Check if there's enough petroleum.
     * @param amount Amount to check for.
     * @return bool True if there's enough petroleum, false otherwise.
     */
    bool hasEnoughPetroleum(int amount) const { return petroleum >= amount; }

    /**
     * @brief Check if there's enough of both currencies to afford a cost.
     * @param cost Currency object representing the cost.
     * @return bool True if there's enough of both currencies, false otherwise.
     */
    bool canAfford(const Currency& cost) const;

    /**
     * @brief Pay a cost using the available currency.
     * This will subtract the cost from the current currency amounts if affordable.
     * @param cost Currency object representing the cost to pay.
     * @return bool True if payment was successful, false if not enough currency.
     */
    bool pay(const Currency& cost);

    // Operator overloads
    /**
     * @brief Addition operator - adds two Currency objects.
     * @param other Currency to add.
     * @return Currency Result of addition.
     */
    Currency operator+(const Currency& other) const;

    /**
     * @brief Subtraction operator - subtracts two Currency objects.
     * @param other Currency to subtract.
     * @return Currency Result of subtraction.
     */
    Currency operator-(const Currency& other) const;

    /**
     * @brief Addition assignment operator.
     * @param other Currency to add.
     * @return Currency& Reference to this Currency object.
     */
    Currency& operator+=(const Currency& other);

    /**
     * @brief Subtraction assignment operator.
     * @param other Currency to subtract.
     * @return Currency& Reference to this Currency object.
     */
    Currency& operator-=(const Currency& other);

    /**
     * @brief Equality operator.
     * @param other Currency to compare with.
     * @return bool True if currencies are equal, false otherwise.
     */
    bool operator==(const Currency& other) const;

    /**
     * @brief Inequality operator.
     * @param other Currency to compare with.
     * @return bool True if currencies are not equal, false otherwise.
     */
    bool operator!=(const Currency& other) const;

    /**
     * @brief Less than operator - checks if this currency is less than other.
     * @param other Currency to compare with.
     * @return bool True if this currency has less of both types, false otherwise.
     */
    bool operator<(const Currency& other) const;

    /**
     * @brief Less than or equal operator.
     * @param other Currency to compare with.
     * @return bool True if this currency has less than or equal amounts of both types.
     */
    bool operator<=(const Currency& other) const;

    /**
     * @brief Greater than operator.
     * @param other Currency to compare with.
     * @return bool True if this currency has more of both types, false otherwise.
     */
    bool operator>(const Currency& other) const;

    /**
     * @brief Greater than or equal operator.
     * @param other Currency to compare with.
     * @return bool True if this currency has greater than or equal amounts of both types.
     */
    bool operator>=(const Currency& other) const;

    /**
     * @brief Scalar multiplication operator.
     * @param multiplier Value to multiply all currencies by.
     * @return Currency Result of multiplication.
     */
    Currency operator*(int multiplier) const;

    /**
     * @brief Scalar multiplication assignment operator.
     * @param multiplier Value to multiply all currencies by.
     * @return Currency& Reference to this Currency object.
     */
    Currency& operator*=(int multiplier);

    // Utility methods
    /**
     * @brief Get a string representation of the currency.
     * @return std::string String representation showing all currency amounts.
     */
    std::string toString() const;

    /**
     * @brief Check if all currency amounts are zero.
     * @return bool True if all amounts are zero, false otherwise.
     */
    bool isEmpty() const;

    /**
     * @brief Reset all currency amounts to zero.
     */
    void clear();
};

// Global operators for scalar multiplication (commutative)
/**
 * @brief Scalar multiplication operator (int * Currency).
 * @param multiplier Value to multiply all currencies by.
 * @param currency Currency object to multiply.
 * @return Currency Result of multiplication.
 */
Currency operator*(int multiplier, const Currency& currency);
