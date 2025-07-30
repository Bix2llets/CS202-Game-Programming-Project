#include <SFML/Graphics.hpp>
#include "Entity/Tower/Tower.hpp"

#include "GUIComponents/button.hpp"
#include "Gameplay/Currency.hpp"

#include "GUIComponents/Mediator.hpp"
class TowerMenu : public Mediator {
    private:
    sf::Sprite basePanel;
    std::vector<std::unique_ptr<Button>> towerButtons;
    std::vector<std::unique_ptr<sf::Texture>> combinedTowerTextures;
    std::vector<std::unique_ptr<sf::Texture>> towerTextures;
    std::vector<sf::RenderTexture> renderTexes;
    const sf::Vector2f position = {1000, 0};
    const sf::Vector2f size = {200, 600};

    const sf::Vector2i buttonGap = {5, 5};
    const int BUTTON_PER_ROW = 2;

    const Currency& budgetRef;
    sf::Text petroleumDisplay;
    sf::Text scrapDisplay;

    sf::Sprite scrapIcon;
    sf::Sprite petrolIcon;

    bool isTowerSelected;

    Mediator* superMediator;
    public:
    TowerMenu(const Currency& budget, Mediator& superMediator);
    void update();

    void render(sf::RenderStates state) const ;

    void onLoad();
    void onUnload();
    private:
    void setResourceDisplay();
    void setTowerButtonDisplay();
    void registerMessages();

};