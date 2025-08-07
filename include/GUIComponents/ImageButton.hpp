#include "GUIComponents/ButtonBase.hpp"

// * Left-middle aligned.
class ImageButton : public ButtonBase {
    private:
    sf::Sprite buttonSprite;
    sf::RectangleShape spriteRenderBox;
    sf::FloatRect boundingBox;

    std::vector<std::string> displayTexts;
    std::vector<sf::Sprite> displaySprites;

    public:
    // Set the position of the button and its elements
    ImageButton& setPosition(const sf::Vector2f& pos);
    ImageButton& setButtonSize(const sf::Vector2f& size);
    ImageButton& setImage(const sf::Texture& texture);
    ImageButton& setImageViewport(const sf::FloatRect& rect);
    ImageButton& setDisplayText(std::size_t line, const std::string& text);
    ImageButton& setDisplaySprite(std::size_t line, const sf::Sprite& sprite);
    ImageButton& setDisplaySprite(std::size_t line, sf::Texture* texture);
    ImageButton& setStyle(nlohmann::json styleConfig);
    ImageButton& adjust();
    const sf::FloatRect& getBoundingBox() const;
    const sf::FloatRect& getRenderBox() const;
    const sf::Sprite& getSprite() const;
    const sf::Sprite& getLineSprite(std::size_t line) const;
};