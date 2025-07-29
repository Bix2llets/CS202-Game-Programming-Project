#include <SFML/Graphics.hpp>
#include "Entity/Tower/Tower.hpp"

#include "GUIComponents/button.hpp"
class TowerMenu : public sf::Drawable {
    private:
    sf::Sprite basePanel;
    std::vector<Button> towerButtons;
    public:
    TowerMenu();


};