/**
 * @class Cursor
 * @brief Singleton class representing the cursor in the game GUI.
 *
 * The Cursor class manages the position and rendering of the cursor, including
 * previewing tower placement. It uses the singleton pattern to ensure only one
 * instance of the cursor exists throughout the application.
 *
 * Usage:
 * - Access the single instance via Cursor::getInstance().
 * - Use setPreviewTower() to show a tower placement preview.
 * - Use removeSprite() to remove the preview sprite.
 *
 * @note This class is intended to be used as a singleton. Direct instantiation
 * is discouraged.
 */
#pragma once
#include <Core/MouseObserver.hpp>
#include <Entity/Tower/Tower.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

class Cursor : public sf::Drawable, public MouseObserver {
    sf::Vector2f position;
    sf::Sprite renderImage;
    static const int cursorRadius = 2;
    bool isDisplaying;
    // Singleton pattern
   private:
    std::string carryingTowerId;
    static std::unique_ptr<Cursor> instance;
    Cursor();
    Cursor(const Cursor&) = delete;
    Cursor& operator=(const Cursor&) = delete;

   public:
    ~Cursor() = default;
    static Cursor& getInstance();

    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    void removeRenderImage();
    void clearCarryingTower();

    void setRenderImage(sf::Sprite sprite);
    void setCarryingTower(std::string id);

    void setPosition(const sf::Vector2f& pos);
    const sf::Vector2f& getPosition() const;

    bool onMouseEvent(Mouse mouse, UserEvent event,
                      const sf::Vector2f& worldPosition,
                      const sf::Vector2f& windowPosition);
    bool onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                       const sf::Vector2f& windowPosition) override;

    inline std::string getCarryingTowerID() { return carryingTowerId; }
};