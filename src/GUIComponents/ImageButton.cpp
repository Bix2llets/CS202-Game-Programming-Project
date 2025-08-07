#include "GUIComponents/ImageButton.hpp"

#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"

#include "Base/constants.hpp"
ImageButton& ImageButton::setPosition(const sf::Vector2f& pos) {
    boundingBox.position = pos;
    return *this;
}

ImageButton& ImageButton::setButtonSize(const sf::Vector2f& size) {
    boundingBox.size = size;
    return *this;
}

ImageButton& ImageButton::setImage(const sf::Texture& texture) {
    spriteRenderBox.setTexture(&texture, true);
    return *this;
}

ImageButton& ImageButton::setImageViewport(const sf::FloatRect& rect) {
    spriteRenderBox.setTextureRect(sf::IntRect(rect));
    return *this;
}

ImageButton& ImageButton::setDisplayText(std::size_t line,
                                         const std::string& text) {
    if (line >= displayTexts.size()) {
        displayTexts.resize(line + 1);
        displaySprites.resize(line + 1, sf::Sprite(GameConstants::BLANK_TEXTURE));
    }
    displayTexts[line] = text;
    return *this;
}

ImageButton& ImageButton::setStyle(nlohmann::json styleConfig) {
    graphicState.loadStyle(styleConfig);
    return *this;
}

ImageButton& ImageButton::setDisplaySprite(std::size_t line,
                                           const sf::Sprite& sprite) {
    if (line >= displaySprites.size()) {
        displaySprites.resize(line + 1, sf::Sprite(GameConstants::BLANK_TEXTURE));
    }
    displaySprites[line] = sprite;
    return *this;
}
 
ImageButton& ImageButton::setDisplaySprite(std::size_t line,
                                           sf::Texture* texture) {
    if (line >= displaySprites.size()) {
        displaySprites.resize(line + 1, sf::Sprite(GameConstants::BLANK_TEXTURE));
    }
    if (texture) {
        displaySprites[line] = sf::Sprite(*texture);
        displaySprites[line] =
            Aligner::align(displaySprites[line], HorizontalAlignment::Left,
                           VerticalAlignment::Middle);
        displaySprites[line] =
            Scaler::scaleSprite(displaySprites[line], {32.f, 32.f});
    }
    return *this;
}

ImageButton& ImageButton::adjust() {
    spriteRenderBox =
        Aligner::align(spriteRenderBox, HorizontalAlignment::Center,
                       VerticalAlignment::Middle);
    int minDim = std::min(boundingBox.size.x, boundingBox.size.y);
    spriteRenderBox.setPosition(sf::Vector2f{(float)minDim, (float)minDim} / 2.f);

    for (std::size_t i = 0; i < displayTexts.size(); ++i) {
        if (i < displaySprites.size()) {
            displaySprites[i] =
                Aligner::align(displaySprites[i], HorizontalAlignment::Left,
                               VerticalAlignment::Middle);
            displaySprites[i].setPosition(
                boundingBox.position +
                sf::Vector2f{static_cast<float>(graphicState.getStyle().getPadding()[0]),
                             static_cast<float>(i * 30)});
        }
        if (!displayTexts[i].empty()) {
            sf::Text text(*graphicState.getStyle().getFont(), displayTexts[i],
                          graphicState.getStyle().getFontSize());
            text.setFillColor(graphicState.getTextColor());
            text = Aligner::align(text, HorizontalAlignment::Left,
                                  VerticalAlignment::Middle);
            text.setPosition({boundingBox.position.x + 10.f,
                              boundingBox.position.y + i * 30.f});
            // Render the text to the button
            // This part is not implemented in this snippet
        }
    }
    return *this;
}