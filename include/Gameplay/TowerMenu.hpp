#include <SFML/Graphics.hpp>
#include "Entity/Tower/Tower.hpp"

#include "GUIComponents/button.hpp"
#include "Gameplay/Currency.hpp"
class TowerMenu {
    private:
    sf::Sprite basePanel;
    std::vector<Button> towerButtons;
    
    const sf::Vector2f position = {1000, 0};
    const sf::Vector2f size = {200, 600};

    const Currency& budgetRef;
    sf::Text petroleumDisplay;
    sf::Text scrapDisplay;

    sf::Sprite scrapIcon;
    sf::Sprite petrolIcon;
    public:
    TowerMenu(const Currency& budget);
    void update();

    void render(sf::RenderStates state) const ;
};