#include <SFML/Graphics.hpp>
#include <string>

#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "GUIComponents/button.hpp"
#include "GUIComponents/mediator.hpp"
#include "Scene/Scene.hpp"

class SceneManager;
class MainMenu : public Scene {
   private:
    Button testBtn;
    Mediator testGroup;

   public:
    MainMenu(sf::RenderWindow &window, const std::string &name, SceneManager& parentManager,
             InputManager &inputManager, ResourceManager &resManager);

    void update();
    void draw(sf::RenderTarget &target, sf::RenderStates state) const;
};