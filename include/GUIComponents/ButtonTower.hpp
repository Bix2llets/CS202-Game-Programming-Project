#pragma once
#include <SFML/Graphics.hpp>

#include "Core/MouseObserver.hpp"
#include "GUIComponents/button.hpp"
// * Card used inside the tower menu

class ButtonTower : public Button {
    friend class ButtonTowerBuilder;

   private:
    std::unique_ptr<sf::Sprite> object;
    bool isHidden;

   public:
   ButtonTower(Mediator& mediator);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void onMousePress(Mouse mouse, UserEvent event,
                      const sf::Vector2f& worldPosition,
                      const sf::Vector2f& windowPosition);

    inline void setHidden(bool status) { isHidden = status; };
};