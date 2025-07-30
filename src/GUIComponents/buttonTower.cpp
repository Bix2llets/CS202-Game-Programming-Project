#include "GUIComponents/ButtonTower.hpp"

#include "Core/MouseState.hpp"
#include "GUIComponents/cursor.hpp"
ButtonTower::ButtonTower(Mediator& mediator) : Button(mediator) {}

void ButtonTower::onMousePress(Mouse mouse, UserEvent event,
                               const sf::Vector2f& worldPosition,
                               const sf::Vector2f& windowPosition) {
    Button::onMouseEvent(mouse, event, worldPosition, windowPosition);

    if (mouse == Mouse::Left && event == UserEvent::Press) {
        if (geometricInfo.contains(windowPosition)) isHidden = !isHidden;

        if (isHidden)
            Cursor::getInstance().setRenderImage(*object);
        else
            Cursor::getInstance().removeRenderImage();
    }
}

void ButtonTower::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    if (!isHidden) {
        Button::draw(target, states);
        target.draw(*object, states);
    }
}