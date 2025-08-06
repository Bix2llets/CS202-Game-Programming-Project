#pragma once

#include "Core/TextInputObserver.hpp"
#include "Core/MouseObserver.hpp"
#include "GUIComponents/Style.hpp"
#include "GUIComponents/GUIState.hpp"

class TextBox : public TextInputObserver, public MouseObserver, public sf::Drawable {
    private:
    GUIState graphicsState;

    sf::Text inputText;
    sf::FloatRect boundingBox;

    int maximumLength = -1;
    public:
    TextBox();
    TextBox& setSize(sf::Vector2f size);
    TextBox& setPosition(sf::Vector2f position);
    TextBox& setText(const std::string& text);
    TextBox& clearText();
    TextBox& setStyle(nlohmann::json styleConfig);
    TextBox& adjust();
    // * Setting maximum length to -1 will make the string's length only limited by the textbox's size
    TextBox& setMaximumLength(int length);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool onMouseEvent(Mouse button, UserEvent event,
                      const sf::Vector2f& worldPosition,
                      const sf::Vector2f& windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                       const sf::Vector2f& windowPosition) override {return false;};
    bool onTextInput(int unicode) override;

    inline std::string getText() const {return inputText.getString();}

    void update();
};