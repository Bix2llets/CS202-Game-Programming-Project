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

    std::unique_ptr<RectangularButton> resolution1;
    std::unique_ptr<RectangularButton> resolution2;
    std::unique_ptr<RectangularButton> resolution3;
    std::unique_ptr<RectangularButton> backButton;

    std::vector<std::unique_ptr<RectangularButton>> alwaysShownElements;

    void createButtons();
    void setupHandlers();
    void setupButtonMessages();
    void setupComponentVector();
};