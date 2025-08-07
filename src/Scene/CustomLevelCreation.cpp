#include "Scene/CustomLevelCreation.hpp"

#include "COre/MouseState.hpp"
#include "Core/InputManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/SceneManager.hpp"
#include "Core/UserEvent.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Utility/Aligner.hpp"
void CustomLevelCreation::createButtons() {
    RectangularButtonBuilder builder(*this);
    incrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({250, 50})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("next"))
            .setCallback([this](RectangularButton* button) {
                notify("Gridsize increase");
            })
            .build());

    incrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({250, 150})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("next"))
            .setCallback([this](RectangularButton* button) {
                notify("Octaves increase");
            })
            .build());

    incrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({250, 250})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("next"))
            .setCallback([this](RectangularButton* button) {
                notify("Persistence increase");
            })
            .build());
    incrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({250, 350})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("next"))
            .setCallback([this](RectangularButton* button) {
                notify("Lacunarity increase");
            })
            .build());
    incrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({250, 450})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("next"))
            .setCallback([this](RectangularButton* button) {
                notify("Depth factor increase");
            })
            .build());
    decrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({50, 50})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("prev"))
            .setCallback([this](RectangularButton* button) {
                notify("Gridsize decrease");
            })
            .build());

    decrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({50, 150})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("prev"))
            .setCallback([this](RectangularButton* button) {
                notify("Octaves decrease");
            })
            .build());

    decrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({50, 250})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("prev"))
            .setCallback([this](RectangularButton* button) {
                notify("Persistence decrease");
            })
            .build());
    decrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({50, 350})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("prev"))
            .setCallback([this](RectangularButton* button) {
                notify("Lacunarity decrease");
            })
            .build());
    decrementButtons.push_back(
        builder.reset()
            .loadJson("borderless_background_basic")
            .setPosition({50, 450})
            .setSize({25, 50})
            .setBackground(ResourceManager::getInstance().getTexture("prev"))
            .setCallback([this](RectangularButton* button) {
                notify("Depth factor decrease");
            })
            .build());

    creationButton = builder.reset()
                         .loadJson("background_basic")
                         .setPosition({800, 800})
                         .setSize({100, 75})
                         .setText("Launch")
                         .setCallback([this](RectangularButton* button) {
                             notify("Launch game");
                         })
                         .build();

    clearButton = builder.reset()
                      .loadJson("background_basic")
                      .setPosition({100, 800})
                      .setSize({100, 75})
                      .setText("Reset parameters")
                      .setCallback([this](RectangularButton* button) {
                          notify("Reset parameters");
                      })
                      .build();

    togglePresetButton = builder.reset()
                             .loadJson("background_basic")
                             .setPosition({300, 800})
                             .setSize({100, 75})
                             .setText("Choose maps")
                             .setCallback([this](RectangularButton* button) {
                                 notify("Toggle preset maps");
                             })
                             .build();

    menuButton = builder.reset()
                     .loadJson("background_basic")
                     .setPosition({0, 0})
                     .setSize({50, 30})
                     .setText("Menu")
                     .setCallback([this](RectangularButton* button) {
                         notify("Return to main menu");
                     })
                     .build();

    seedBox.setPosition({500, 800});
    seedBox.setSize({200, 75});
    seedBox.setText("Type seed");
    seedBox.setStyle(
        JSONLoader::getInstance().getStyle("borderless_background_basic"));
    seedBox.setMaximumLength(10);
    seedBox.adjust();
}

void CustomLevelCreation::draw(sf::RenderTarget& target,
                               sf::RenderStates states) const {
    Window::getInstance().toggleGUIMode();
    for (const auto& button : incrementButtons) {
        target.draw(*button, states);
    }
    for (const auto& button : decrementButtons) {
        target.draw(*button, states);
    }

    for (const auto& text : parameterTexts) {
        target.draw(text, states);
        sf::CircleShape circle(2);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(text.getPosition());
        target.draw(circle, states);
    }
    target.draw(*creationButton, states);
    target.draw(*clearButton, states);
    target.draw(*togglePresetButton, states);
    target.draw(*menuButton, states);
    target.draw(seedBox, states);
}

void CustomLevelCreation::update() {
    for (const auto& button : incrementButtons) {
        button->update();
    }
    for (const auto& button : decrementButtons) {
        button->update();
    }
    creationButton->update();
    clearButton->update();
    togglePresetButton->update();
    menuButton->update();
    seedBox.update();
}

CustomLevelCreation::CustomLevelCreation() {
    subscribeEvents();
    createButtons();
    createTexts();
}

void CustomLevelCreation::onLoad() {
    subscribeMouse(Mouse::Left, UserEvent::Press,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::None, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Left, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Right, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Middle, UserEvent::Move,
                   InputManager::getInstance().getMouseState());
    subscribeMouse(Mouse::Left, UserEvent::Release,
                   InputManager::getInstance().getMouseState());
    for (const auto& button : incrementButtons) {
        button->update();
    }
    for (const auto& button : decrementButtons) {
        button->resetAnimation();
    }
    creationButton->resetAnimation();
    clearButton->resetAnimation();
    togglePresetButton->resetAnimation();
    menuButton->resetAnimation();
    seedBox.resetAnimation();
}

void CustomLevelCreation::onUnload() {
    unSubscribeMouse(Mouse::Left, UserEvent::Press,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::None, UserEvent::Move,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::Left, UserEvent::Move,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::Right, UserEvent::Move,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::Middle, UserEvent::Move,
                     InputManager::getInstance().getMouseState());
    unSubscribeMouse(Mouse::Left, UserEvent::Release,
                     InputManager::getInstance().getMouseState());
}

bool CustomLevelCreation::onMouseEvent(Mouse button, UserEvent event,
                                       const sf::Vector2f& worldPosition,
                                       const sf::Vector2f& windowPosition) {
    bool isProcessed = false;
    for (const auto& btn : incrementButtons) {
        isProcessed |=
            btn->onMouseEvent(button, event, worldPosition, windowPosition);
    }
    for (const auto& btn : decrementButtons) {
        isProcessed |=
            btn->onMouseEvent(button, event, worldPosition, windowPosition);
    }
    isProcessed |= creationButton->onMouseEvent(button, event, worldPosition,
                                                windowPosition);

    isProcessed |=
        clearButton->onMouseEvent(button, event, worldPosition, windowPosition);
    isProcessed |= togglePresetButton->onMouseEvent(
        button, event, worldPosition, windowPosition);
    isProcessed |=
        menuButton->onMouseEvent(button, event, worldPosition, windowPosition);
    isProcessed |=
        seedBox.onMouseEvent(button, event, worldPosition, windowPosition);
    return isProcessed;
}

bool CustomLevelCreation::onScrollEvent(float delta,
                                        const sf::Vector2f& worldPosition,
                                        const sf::Vector2f& windowPosition) {
    // Handle scroll events if needed
    return false;
}

void CustomLevelCreation::subscribeEvents() {
    subscribe("Return to main menu", [this](std::any sender, std::any data) {
        SceneManager::getInstance().changeScene("Main menu");
    });
}

void CustomLevelCreation::createTexts() {
    parameterTexts.clear();

    for (int i = 0; i < incrementButtons.size(); i++) {
        sf::Text text(*ResourceManager::getInstance().getFont("pixel"));
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::Black);
        text.setString("Testing");
        text = Aligner::align(text, HorizontalAlignment::Center,
                              VerticalAlignment::Middle);
        text.setPosition(
            sf::Vector2f{(incrementButtons[i]->getBounds().position.x +
                          incrementButtons[i]->getBounds().size.x +
                          decrementButtons[i]->getBounds().position.x) /
                             2,
                         incrementButtons[i]->getBounds().position.y +
                             incrementButtons[i]->getBounds().size.y / 2});
        parameterTexts.push_back(std::move(text));
    }
}