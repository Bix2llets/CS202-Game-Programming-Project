#pragma once

#include <vector>

#include "GUIComponents/RectangularButton.hpp"
#include "Scene/Scene.hpp"
class Setting : public Scene {
   public:
    Setting();

    void onLoad() override;
    void onUnload() override;

    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

   private:
    std::unique_ptr<RectangularButton> musicVolumeIncrement;
    std::unique_ptr<RectangularButton> musicVolumeDecrement;
    std::unique_ptr<RectangularButton> soundVolumeIncrement;
    std::unique_ptr<RectangularButton> soundVolumeDecrement;

    std::unique_ptr<RectangularButton> backButton;
    sf::Text musicVolumeDisplay;
    sf::Text soundVolumeDisplay;

    std::vector<RectangularButton*> alwaysShownElements;

    void createButtons();
    void setupHandlers();
    void setupButtonMessages();
    void setupComponentVector();
};