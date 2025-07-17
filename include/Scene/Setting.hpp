#pragma once

#include <vector>

#include "GUIComponents/button.hpp"
#include "Scene/Scene.hpp"
class Setting : public Scene {
   public:
    Setting(SceneManager &parentManager,
            InputManager &inputManager, ResourceManager &resManager);

    void onLoad() override;
    void onUnload() override;

    void update() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

   private:
    std::unique_ptr<Button> musicVolumeIncrement;
    std::unique_ptr<Button> musicVolumeDecrement;
    std::unique_ptr<Button> soundVolumeIncrement;
    std::unique_ptr<Button> soundVolumeDecrement;

    std::unique_ptr<Button> resolution1;
    std::unique_ptr<Button> resolution2;
    std::unique_ptr<Button> resolution3;
    std::unique_ptr<Button> backButton;

    std::vector<std::unique_ptr<Button>> alwaysShownElements;

    void createButtons();
    void setupHandlers();
    void setupButtonMessages();
    void setupComponentVector();
};