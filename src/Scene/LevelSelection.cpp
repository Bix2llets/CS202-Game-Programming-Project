#include "Scene/LevelSelection.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/LevelFactory.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/TextJustifier.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"
LevelSelection::LevelSelection()
    : title(*ResourceManager::getInstance().getFont("pixel")),
      buttonBackgroundSprite(*ResourceManager::getInstance().getTexture("level_selection_background")) {
    createButtons();
    createBackgrounds();
    createTexts();
    subscribeEvents();
    levelPreviewBackground = std::make_unique<sf::Sprite>(
        *ResourceManager::getInstance().getTexture("preview_background"));

    Scaler::scaleSprite(*levelPreviewBackground,
                        sf::Vector2f{500 + 40, 375 + 40});
    Aligner::align(*levelPreviewBackground);
    levelPreviewBackground->setPosition(
        sf::Vector2f(800, GameConstants::DEFAULT_WINDOW_HEIGHT / 2));
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
                .setPosition(sf::Vector2f(
                    100, GameConstants::DEFAULT_WINDOW_HEIGHT / 2 -
                             (previewButtonSize.y * levelCount +
                              verticalSpace * (levelCount - 1)) /
                                 2 +
                             index * (previewButtonSize.y + verticalSpace)))
                .setSize(previewButtonSize)
                .setCallback([levelID, this](RectangularButton* btn) {
                    notify("Level selected", btn, levelID);
                })
                .loadJson("borderless_background_black")
                .setBackground(ResourceManager::getInstance().getTexture(
                    "tower_menu_background"))
                .build();
        levelButtons.push_back(std::move(button));
        index++;
    }
    backButton =
        builder.reset()
            .setPosition(sf::Vector2f(30, 30))
            .setSize(sf::Vector2f(132, 60))
            .setCallback([this](RectangularButton* btn) { notify("Back"); })
            .loadJson("borderless_background_black")
            .setBackground(
                ResourceManager::getInstance().getTexture("back_button"))
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
            Scaler::scaleSprite(*sprite, sf::Vector2f{500, 375});
            Aligner::align(*sprite);
            sprite->setPosition(
                sf::Vector2f(800, GameConstants::DEFAULT_WINDOW_HEIGHT / 2));

            levelBackgrounds.push_back(std::move(sprite));
        } else {
            Logger::error(
                "Failed to load background texture, using blank texture");
        }
    }

    buttonBackgroundSprite.setPosition(sf::Vector2f(0, GameConstants::DEFAULT_WINDOW_HEIGHT / 2.f));
    Scaler::scaleSprite(buttonBackgroundSprite, sf::Vector2f(380, GameConstants::DEFAULT_WINDOW_HEIGHT * 10));
    Aligner::align(buttonBackgroundSprite, HorizontalAlignment::Left, VerticalAlignment::Middle);
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
    target.draw(buttonBackgroundSprite, state);
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
                            target.draw(levelDescriptions[i], state);
                            target.draw(levelTitles[i], state);
                            target.draw(*levelPreviewBackground, state);
        }
        target.draw(*levelButtons[i]);
    }
    target.draw(*backButton, state);
    target.draw(title, state);
}

bool LevelSelection::onMouseEvent(Mouse button, UserEvent event,
                                  const sf::Vector2f& worldPosition,
                                  const sf::Vector2f& windowPosition) {
    for (auto& btn : levelButtons) {
        if (btn->onMouseEvent(button, event, worldPosition, windowPosition)) {
            return true;
        }
    }
    if (backButton->onMouseEvent(button, event, worldPosition,
                                 windowPosition)) {
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
    subscribe("Level selected", [this](std::any sender, std::any data) {
        try {
            std::string levelID = std::any_cast<std::string>(data);
            SceneManager::getInstance().enqueueSceneAdd(
                "Gameplay", LevelFactory::getInstance().getLevel(levelID));
            SceneManager::getInstance().enqueueSceneChange("Gameplay");
        } catch (const std::bad_any_cast& e) {
            Logger::error("Failed to cast level ID: " + std::string(e.what()));
        }
    });
    subscribe("Back", [this](std::any sender, std::any data) {
        SceneManager::getInstance().enqueueSceneChange("Main menu");
    });
}

void LevelSelection::createTexts() {
    auto font = ResourceManager::getInstance().getFont("text");
    if (!font) {
        Logger::error("Failed to load default font for LevelSelection");
        return;
    }

    title =

        TextJustifier::createJustified(
            "Level Selection", *font, 40,
            GameConstants::DEFAULT_WINDOW_WIDTH - 500.f, sf::Color::Black);

    Aligner::align(title);
    title.setPosition(
        sf::Vector2f((380 + GameConstants::DEFAULT_WINDOW_WIDTH) / 2, title.getGlobalBounds().size.y + 20));
    title.setFillColor(sf::Color::White);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);

    for (auto [id, jsonFile]: JSONLoader::getInstance().getAllLevels()) {
        sf::Text description(*font);
        description.setFillColor(sf::Color::Black);
        description.setOutlineColor(sf::Color::White);
        // description.setOutlineThickness(1);
        std::string content;
        if (jsonFile.contains("description")) {
            content = (jsonFile["description"].get<std::string>());
        } else {
            content = "No description available";
        }

        description = TextJustifier::createJustified(
            content, *font, 16, 500.f, sf::Color::Black);

        Aligner::align(description);
        description.setPosition(
            sf::Vector2f(800, 750));
        
        levelDescriptions.push_back(description);
    }

    for (auto [id, jsonFile]: JSONLoader::getInstance().getAllLevels()) {
        sf::Text titleText(*font);
        // titleText.setOutlineThickness(1);
        std::string content;
        if (jsonFile.contains("name")) {
            content = (jsonFile["name"].get<std::string>());
        } else {
            content = "No name available";
        }

        titleText = TextJustifier::createJustified(
            content, *font, 28, 500.f, sf::Color::Black);

        Aligner::align(titleText);
        titleText.setPosition(
            sf::Vector2f(800, 225));
        
        levelTitles.push_back(titleText);
    }
}