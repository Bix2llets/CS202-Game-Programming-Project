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
#include <Entity/Tower/Tower.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <Core/MouseObserver.hpp>

class Cursor : public sf::Drawable, public MouseObserver {
    sf::Vector2f position;
    std::unique_ptr<Tower> previewTower;
    static const int radius = 2;

    // Singleton pattern
   private:
    static std::unique_ptr<Cursor> instance;
    Cursor();
    Cursor(const Cursor&) = delete;
    Cursor& operator=(const Cursor&) = delete;
    
    public:
    ~Cursor() = default;
    static Cursor* getInstance();

    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    void removeTower();
    void setTower(const std::string& id);
    void setPosition(const sf::Vector2f& pos);
    const sf::Vector2f& getPosition() const;

    void onMouseEvent(Mouse mouse, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition);
};