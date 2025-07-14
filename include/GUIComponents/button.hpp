#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>

#include "Core/MouseObserver.hpp"
#include "GUIComponents/mediator.hpp"
#include "GUIComponents/Style.hpp"
#include "Entity/Modules/Timer.hpp"

/**
 * @class Button
 * @brief A clickable UI button that notifies a mediator and supports custom
 * callbacks.
 *
 * The Button class represents a simple UI button. When clicked, it notifies the
 * associated mediator and can also invoke a user-defined callback. The button
 * can be drawn using SFML.
 */
class Button : public sf::Drawable, public MouseObserver {
    friend class ButtonBuilder;
   public:
    /**
     * @brief Constructs a Button with a label, position, and mediator
     * reference.
     * @param label The text label of the button.
     * @param position The position of the button in the window.
     * @param mediator Reference to the mediator for event notification.
     */
    Button(const std::string& label, sf::FloatRect geometricInfo, Mediator& mediator);

    /**
     * @brief Sets a callback to be called when the button is clicked.
     * @param callback The function to call on click.
     */
    void setOnClick(const std::function<void(Button*)>& callback);

    /**
     * @brief Sets the string to be notification message when notifying the mediator
     * @param str the string to be used as message.
     */
    void setNotificationMessage(const std::string &str);

    /**
     * @brief Simulates a button click: notifies the mediator and calls the
     * callback.
     */
    void click();

    /**
     * @brief Draws the button to the given render target.
     * @param target The render target.
     * @param states The render states.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * @brief Gets the button's label.
     * @return The label string.
     */
    std::string getLabel() const;

    /**
     * @brief Gets the button's position.
     * @return The position vector.
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Called when a mouse button is pressed.
     * @param button The button that is pressed
     * @param event The button that user trigers
     * @param worldPosition The mouse position in world coordinates.
     * @param windowPosition The mouse position in window coordinates.
     */
    virtual void onMouseEvent(Mouse button, UserEvent event,
                              const sf::Vector2f& worldPosition,
                              const sf::Vector2f& windowPosition);
        
    void subscribeMouseAll(MouseState &mouseState);
    void unSubscribeMouseAll(MouseState &mouseState);
    void update();

   private:
   // * Graphic part
    std::unique_ptr<sf::Text> label;
    sf::FloatRect geometricInfo;
    Style style;

    Timer hover;
    Timer press;
    Timer reverseHover;
    Timer reversePress;

    bool isPressed;
    bool isHovered;
    // * Communication part
    Mediator& mediator;
    std::function<void(Button*)> onClick;
    std::string onClickMessage;
};
