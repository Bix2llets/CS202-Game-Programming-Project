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
#include "Core/Window.hpp"
#include "Core/SceneManager.hpp"
#include "Core/LevelFactory.hpp"
LevelSelection::LevelSelection() {
    createButtons();
    createBackgrounds();
    subscribeEvents();
}

void LevelSelection::createButtons() {
    RectangularButtonBuilder builder(*this);
    int index = 0;

    const sf::Vector2f previewButtonSize{250, 80};
    const float verticalSpace = 20.f;
    int levelCount = JSONLoader::getInstance().getAllLevels().size();
    for (auto [key, levelConfig] : JSONLoader::getInstance().getAllLevels()) {
        std::string levelID;
        if (levelConfig.contains("id"))
            levelID = levelConfig["id"];
        else
            levelID = "default_level";
        auto button =
            builder.reset()
                .setText(levelConfig.contains("name") ? levelConfig["name"]
                                                      : "")
                .setPosition(
                    sf::Vector2f(100, GameConstants::DEFAULT_WINDOW_HEIGHT / 2 - (previewButtonSize.y * levelCount + verticalSpace * (levelCount - 1)) / 2 + 
                                          index * (previewButtonSize.y + verticalSpace)))
                .setSize(previewButtonSize)
                .setCallback([levelID, this](RectangularButton* btn) {
                    notify("Level selected", btn, levelID);
                })
                .loadJson("borderless_background_black")
                .setBackground(
                    ResourceManager::getInstance().getTexture(
                        "tower_menu_background"))
                .build();
        levelButtons.push_back(std::move(button));
        index++;
    }
    backButton = builder.reset()
                         .setPosition(sf::Vector2f(70, 70))
                         .setSize(sf::Vector2f(176, 80))
                         .setCallback([this](RectangularButton* btn) {
                             notify("Back");
                         })
                         .loadJson("borderless_background_black")
                         .setBackground(
                             ResourceManager::getInstance().getTexture(
                                 "back_button"))
                         .build();
}



void LevelSelection::createBackgrounds() {
    for (auto [key, levelConfig] : JSONLoader::getInstance().getAllLevels()) {
        Logger::debug("Working on the level config: " + levelConfig.dump(4));
        auto texture = levelConfig.contains("background")
                           ? ResourceManager::getInstance().getTexture(
                                 levelConfig["background"])
                           : &GameConstants::BLANK_TEXTURE;
        if (levelConfig.contains("background"))
            Logger::debug(
                std::format("Loading background texture : {}",
                            levelConfig["background"].get<std::string>()));
        else
            Logger::debug(
                "No background texture specified, using blank texture");
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
    backButton->update();
}

void LevelSelection::draw(sf::RenderTarget& target,
                          sf::RenderStates state) const {
    Window::getInstance().toggleGUIMode();
    for (int i = 0; i < levelButtons.size(); i++) {
        if (levelButtons[i]->isHovered()) {
            target.draw(*levelBackgrounds[i]);
            Logger::debug(
                std::format("Drawing the {}-th background, at position {} {}, "
                            "size {} {} and origin {} {}",
                            i, levelBackgrounds[i]->getPosition().x,
                            levelBackgrounds[i]->getPosition().y,
                            levelBackgrounds[i]->getGlobalBounds().size.x,
                            levelBackgrounds[i]->getGlobalBounds().size.y,
                            levelBackgrounds[i]->getOrigin().x,
                            levelBackgrounds[i]->getOrigin().y));
        }
        target.draw(*levelButtons[i]);
    }
    backButton->draw(target, state);
}

bool LevelSelection::onMouseEvent(Mouse button, UserEvent event,
                                  const sf::Vector2f& worldPosition,
                                  const sf::Vector2f& windowPosition) {
    for (auto& btn : levelButtons) {
        if (btn->onMouseEvent(button, event, worldPosition, windowPosition)) {
            return true;
        }
    }
    if (backButton->onMouseEvent(button, event, worldPosition, windowPosition)) {
        return true;
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

    for (auto& btn : levelButtons) {
        btn->resetAnimation();
    }
    backButton->resetAnimation();
}

void LevelSelection::subscribeEvents() {
    subscribe("Level selected",
               [this](std::any sender, std::any data) {
                   try {
                       std::string levelID = std::any_cast<std::string>(data);
                       SceneManager::getInstance().enqueueSceneAdd("Gameplay", LevelFactory::getInstance().getLevel(levelID));
                       SceneManager::getInstance().enqueueSceneChange("Gameplay");
                   } catch (const std::bad_any_cast& e) {
                       Logger::error("Failed to cast level ID: " + std::string(e.what()));
                   }
               });
    subscribe("Back",
               [this](std::any sender, std::any data) {
                   SceneManager::getInstance().enqueueSceneChange("Main menu");
               });
}