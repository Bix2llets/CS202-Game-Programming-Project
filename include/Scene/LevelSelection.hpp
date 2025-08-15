#include "Scene/Scene.hpp"

#include <vector>
#include "GUIComponents/RectangularButton.hpp"
#include <memory>
#include "Core/MouseObserver.hpp"
class LevelSelection : public Scene, public MouseObserver {

    private:
    std::vector<std::unique_ptr<RectangularButton>> levelButtons;
    std::vector<std::unique_ptr<sf::Sprite>> levelBackgrounds;
    std::unique_ptr<RectangularButton> backButton;
    std::unique_ptr<sf::Sprite> background;
    std::unique_ptr<sf::Sprite> levelPreviewBackground;
    std::vector<sf::Text> levelDescriptions;
    std::vector<sf::Text> levelTitles;
    sf::Text title;
    void createButtons();
    void createBackgrounds();
    void createTexts();
    void subscribeEvents();
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