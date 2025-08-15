#include "Scene/LevelSelection.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/UserEvent.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
LevelSelection::LevelSelection() {
    createButtons();
    createBackgrounds();
}

void LevelSelection::createButtons() {
    RectangularButtonBuilder builder(*this);
    int index = 0;
    for (nlohmann::json levelConfig :
         JSONLoader::getInstance().getAllLevels()) {
        std::string levelID;
        if (levelConfig.contains("id"))
            levelID = levelConfig["id"];
        else
            levelID = "default_level";
        auto button = builder.reset()
                          .setText(levelConfig.contains("name")
                                       ? levelConfig["name"]
                                       : "")
                          .setPosition(sf::Vector2f(100, 100 + index * 60))
                          .setSize(sf::Vector2f(200, 50))
                          .setCallback([levelID, this](RectangularButton* btn) {
                              notify("Level selected", btn, levelID);
                          })
                          .loadJson("background_basic")
                          .build();
        levelButtons.push_back(std::move(button));
        index++;
    }
}

void LevelSelection::createBackgrounds() {
    for (nlohmann::json levelConfig :
         JSONLoader::getInstance().getAllLevels()) {
        auto texture = levelConfig.contains("background")
                           ? ResourceManager::getInstance().getTexture(
                                 levelConfig["background"])
                           : &GameConstants::BLANK_TEXTURE;
        if (texture) {
            auto sprite = std::make_unique<sf::Sprite>(*texture);
            Scaler::scaleSprite(*sprite, sf::Vector2f{400, 300});
            Aligner::align(*sprite);
            sprite->setPosition(
                sf::Vector2f(800, GameConstants::DEFAULT_WINDOW_HEIGHT / 2));

            levelBackgrounds.push_back(std::move(sprite));
        } else {
            Logger::error(
                "Failed to load background texture, using blank texture");
        }
    }
}

void LevelSelection::update() {
    for (auto& button : levelButtons) {
        button->update();
    }
}

void LevelSelection::draw(sf::RenderTarget& target,
                          sf::RenderStates state) const {
    for (int i = 0; i < levelButtons.size(); i++) {
        if (levelButtons[i]->isHovered()) {
            target.draw(*levelBackgrounds[i]);
        }
        target.draw(*levelButtons[i]);
    }
}

bool LevelSelection::onMouseEvent(Mouse button, UserEvent event,
                                  const sf::Vector2f& worldPosition,
                                  const sf::Vector2f& windowPosition) {
    for (auto& btn : levelButtons) {
        if (btn->onMouseEvent(button, event, worldPosition, windowPosition)) {
            return true;
        }
    }
    return false;
}

bool LevelSelection::onScrollEvent(float delta,
                                   const sf::Vector2f& worldPosition,
                                   const sf::Vector2f& windowPosition) {
    return false;
}

LevelSelection::~LevelSelection() {
    // Destructor logic if needed
}

void LevelSelection::onLoad() {
    subscribeMouse(Mouse::Left, UserEvent::Press,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Left, UserEvent::Release,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::None, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
}

void LevelSelection::onUnload() {
    unSubscribeMouse(Mouse::Left, UserEvent::Press,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::Left, UserEvent::Release,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::None, UserEvent::Move,
                     InputManager::getInstance().getMouseState());
}