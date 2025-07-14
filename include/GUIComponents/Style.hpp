/**
 * @brief Provides style settings for GUI components.
 *
 * The Style class encapsulates visual styling properties that can be applied to
 * GUI elements, such as colors, fonts, borders, and other appearance-related
 * attributes.
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <json.hpp>

class ResourceManager;
struct ColorSet {
    sf::Color background;
    sf::Color text;
    sf::Color border;
};
struct Style {
    private:

    std::string id;
    ColorSet normal;
    ColorSet hover;
    ColorSet click;
    const sf::Font* font;
    std::array<int, 2> padding;

    int borderWidth;
    int borderRadius;

    public:
    inline const std::string& getId() const { return id; }
    inline const ColorSet& getNormal() const { return normal; }
    inline const ColorSet& getHover() const { return hover; }
    inline const ColorSet& getClick() const { return click; }
    inline const sf::Font* getFont() const { return font; }
    inline const std::array<int, 2>& getPadding() const { return padding; }
    inline const int getBorderWidth() const { return borderWidth; }
    inline const int getBorderRadius() const { return borderRadius; }
    inline const ColorSet getNormalSet() const {return normal;}
    inline const ColorSet getHoverSet() const {return hover;}
    inline const ColorSet getClickSet() const {return click;}
    Style() = default;
    void loadJson(const nlohmann::json &file, const ResourceManager &manager);
};