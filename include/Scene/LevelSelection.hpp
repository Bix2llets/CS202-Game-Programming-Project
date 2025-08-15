#include "Scene/Scene.hpp"

#include <vector>
#include "GUIComponents/RectangularButton.hpp"
#include <memory>
#include "Core/MouseObserver.hpp"
class LevelSelection : public Scene, public MouseObserver {

    private:
    std::vector<std::unique_ptr<RectangularButton>> levelButtons;
    std::vector<std::unique_ptr<sf::Sprite>> levelBackgrounds;
    
    void createButtons();
    void createBackgrounds();
    public:
    LevelSelection();
    ~LevelSelection();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    void onLoad() override;
    void onUnload() override;
    bool onMouseEvent(Mouse button, UserEvent event,
                              const sf::Vector2f& worldPosition,
                              const sf::Vector2f& windowPosition) override; 
    
    bool onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                                      const sf::Vector2f& windowPosition) override; 
    
};