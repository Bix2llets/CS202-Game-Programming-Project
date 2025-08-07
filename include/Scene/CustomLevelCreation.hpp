#include "Scene/Scene.hpp"

#include "GUIComponents/RectangularButton.hpp"
#include "GUIComponents/Textbox.hpp"
#include "Gameplay/TerrainParameter.hpp"
class CustomLevelCreation : public Scene, public MouseObserver {
    private:
    std::vector<std::unique_ptr<RectangularButton>> incrementButtons;
    std::vector<std::unique_ptr<RectangularButton>> decrementButtons;
    std::vector<sf::Text> parameterTexts;
    std::vector<sf::Text> parameterDescriptions;

    TextBox seedBox;

    TerrainParameters terrainParameters;
    std::unique_ptr<RectangularButton> creationButton;
    std::unique_ptr<RectangularButton> clearButton;
    std::unique_ptr<RectangularButton> togglePresetButton;
    std::unique_ptr<RectangularButton> menuButton;

    void createButtons();
    void subscribeEvents();
    void createTexts();

    public:
    
    
    CustomLevelCreation();
    void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
    void update() override;
    bool onMouseEvent(Mouse button, UserEvent event,
                              const sf::Vector2f &worldPosition,
                              const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition,
                       const sf::Vector2f &windowPosition) override;

    void onLoad() override;
    void onUnload() override;
    
};