#include "Scene/Overlays/DifficultySelection.hpp"

#include "Base/Constants.hpp"
#include "Core/InputManager.hpp"
#include "Core/MouseState.hpp"
#include "Core/Window.hpp"
#include "Core/JSONLoader.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "Utility/aligner.hpp"
#include "Utility/logger.hpp"

void DifficultySelection::loadDifficulties() {
    // Load difficulty levels from JSON
    try {
        std::unordered_map<std::string, nlohmann::json> difficultiesMap = JSONLoader::getInstance().getAllDifficulties();
        for (auto [diffId, jsonFile] : difficultiesMap) {
            try {
                DifficultyLevel difficulty;
                difficulty.loadFromJson(jsonFile);
                difficulties.push_back(difficulty);
                Logger::success("Loaded difficulty: " + difficulty.getName());
            } catch (const std::exception& e) {
                Logger::error("Failed to load difficulty '" + diffId + "': " + e.what());
            }
        }
        Logger::debug("Total difficulties loaded: " + std::to_string(difficulties.size()));
        std::sort(difficulties.begin(), difficulties.end(), [](const DifficultyLevel& a, const DifficultyLevel& b) {
            return a.getIndex() < b.getIndex();
        });

        for (const auto& diff : difficulties) {
            Logger::debug("Difficulty available: " + diff.getName() + " (ID: " + diff.getId() + ", Index: " + std::to_string(diff.getIndex()) + ")");
        }
    } catch (const std::exception& e) {
        Logger::error("Failed to load difficulties map: " + std::string(e.what()));
        // Fallback: create default difficulties if loading fails
        try {
            DifficultyLevel defaultDifficulty;
            defaultDifficulty.loadFromConfigFile("medium");
            difficulties.push_back(defaultDifficulty);
        } catch (...) {
            Logger::critical("Cannot load any difficulties!");
        }
    }
}

void DifficultySelection::updateDifficultyDisplay() {
    if (difficulties.empty()) {
        currentDifficultyText.setString("No Difficulties Available");
        Aligner::align(currentDifficultyText, HorizontalAlignment::Center, VerticalAlignment::Middle);
        currentDifficultyText.setPosition(background.getPosition());
        return;
    }
    
    if (currentDifficultyIndex >= difficulties.size()) {
        currentDifficultyIndex = 0; // Reset to first difficulty if index is invalid
    }
    
    if (currentDifficultyIndex < difficulties.size()) {
        const DifficultyLevel& currentDifficulty = difficulties[currentDifficultyIndex];
        currentDifficultyText.setString(currentDifficulty.getName());
        
        // Center the text
        Aligner::align(currentDifficultyText, HorizontalAlignment::Center, VerticalAlignment::Middle);
        currentDifficultyText.setPosition(background.getPosition());
    }
}

void DifficultySelection::cycleToPrevious() {
    if (difficulties.empty()) return;
    
    if (currentDifficultyIndex == 0) {
        currentDifficultyIndex = difficulties.size() - 1;
    } else {
        currentDifficultyIndex--;
    }
    updateDifficultyDisplay();
    Logger::debug("Cycled to previous difficulty: " + difficulties[currentDifficultyIndex].getName());
}

void DifficultySelection::cycleToNext() {
    if (difficulties.empty()) return;
    
    currentDifficultyIndex = (currentDifficultyIndex + 1) % difficulties.size();
    updateDifficultyDisplay();
    Logger::debug("Cycled to next difficulty: " + difficulties[currentDifficultyIndex].getName());
}

void DifficultySelection::constructButtons() {
    RectangularButtonBuilder builder(*this);
    
    // Previous button (left arrow)
    auto previousButton = builder.reset()
        .setSize({80.f, 80.f})
        .setPosition(sf::Vector2f{background.getPosition().x - 320.f, background.getPosition().y} - sf::Vector2f{40.f, 40.f})
        .setText("<")
        .loadJson("borderless_background_basic")
        .setCallback([this](RectangularButton *button) {
            cycleToPrevious();
        })
        .setBackground(ResourceManager::getInstance().getTexture("backward_button"))
        .setTextSize(30)
        .build();
    
    // Next button (right arrow)
    auto nextButton = builder.reset()
        .setSize({80.f, 80.f})
        .setPosition(sf::Vector2f{background.getPosition().x + 320.f, background.getPosition().y} - sf::Vector2f{40.f, 40.f})
        .setText(">")
        .loadJson("borderless_background_basic")
        .setCallback([this](RectangularButton *button) {
            cycleToNext();
        })
        .setBackground(ResourceManager::getInstance().getTexture("forward_button"))
        .setTextSize(30)
        .build();
    
    // Start button (confirm selection)
    auto startButton = builder.reset()
        .setSize({200.f, 80.f})
        .setPosition({background.getPosition().x + background.getSize().x /2 - 200.f - 80.f, background.getPosition().y + background.getSize().y / 2 - 80.f - 60.f})
        .setText("Start")
        .loadJson("borderless_background_basic")
        .setCallback([this](RectangularButton *button) {
            if (currentDifficultyIndex < difficulties.size()) {
                const DifficultyLevel& selectedDifficulty = difficulties[currentDifficultyIndex];
                Logger::debug("Starting level with difficulty: " + selectedDifficulty.getName());
                parentScene.notify("start_level_with_difficulty", this, std::make_pair(selectedLevelId, selectedDifficulty.getId()));
            }
        })
        .setBackground(ResourceManager::getInstance().getTexture("button_wooden"))
        .setTextSize(28)
        .build();
    
    // Back button
    auto backButton = builder.reset()
        .setSize({200.f, 80.f})
        .setPosition({background.getPosition().x - background.getSize().x / 2 + 80.f, background.getPosition().y + background.getSize().y / 2 - 80.f - 60.f})
        .setText("Back")
        .loadJson("borderless_background_basic")
        .setCallback([this](RectangularButton *button) {
            Logger::debug("Back button pressed");
            parentScene.notify("close_difficulty_selection");
        })
        .setBackground(ResourceManager::getInstance().getTexture("button_wooden"))
        .setTextSize(28)
        .build();
    
    buttons.push_back(std::move(previousButton));
    buttons.push_back(std::move(nextButton));
    buttons.push_back(std::move(startButton));
    buttons.push_back(std::move(backButton));
}

DifficultySelection::DifficultySelection(Mediator &mediator, const std::string& levelId)
    : Overlay(mediator),
      currentDifficultyText{*ResourceManager::getInstance().getFont("pixel")},
      selectedLevelId(levelId),
      currentDifficultyIndex(0) {
      
    // Setup background
    background.setFillColor(sf::Color(0xFFFFFFFF));
    background.setSize({900, 600});
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    darkOverlay.setSize({GameConstants::DEFAULT_WINDOW_WIDTH,
                         GameConstants::DEFAULT_WINDOW_HEIGHT});
    
    // Center the background
    Aligner::align(background, HorizontalAlignment::Center,
                   VerticalAlignment::Middle);
    background.setPosition({GameConstants::DEFAULT_WINDOW_WIDTH / 2.f,
                            GameConstants::DEFAULT_WINDOW_HEIGHT / 2.f});
    background.setTexture(ResourceManager::getInstance().getTexture("pause_background"));
    
    // Setup title
    title.setString("Select Difficulty");
    title.setCharacterSize(96);
    title.setFillColor(sf::Color(0xBD4C14FF));
    title.setOutlineColor(sf::Color(0x39270FFF));
    title.setOutlineThickness(2);
    title.setFont(*ResourceManager::getInstance().getFont("pixel"));
    Aligner::align(title, HorizontalAlignment::Center, VerticalAlignment::Middle);
    title.setPosition(background.getPosition() - sf::Vector2f{0, background.getSize().y / 2.f - 80.f});
    
    // Setup current difficulty text
    currentDifficultyText.setFillColor(sf::Color(0xE3D8BCFF));
    currentDifficultyText.setCharacterSize(32);
    currentDifficultyText.setFont(*ResourceManager::getInstance().getFont("text"));
    currentDifficultyText.setOutlineColor(sf::Color::Black);
    currentDifficultyText.setOutlineThickness(1);
    
    // Load difficulties and construct buttons
    loadDifficulties();
    updateDifficultyDisplay(); // Set initial difficulty display
    constructButtons();
}

DifficultySelection::~DifficultySelection() {
    buttons.clear();
}

bool DifficultySelection::onMouseEvent(Mouse mouse, UserEvent event,
                                      const sf::Vector2f &worldPosition,
                                      const sf::Vector2f &windowPosition) {
    bool handled = false;
    for (const auto &button : buttons) {
        handled |= button->onMouseEvent(mouse, event, worldPosition, windowPosition);
    }
    return handled;
}

void DifficultySelection::update() {
    for (const auto &button : buttons) {
        button->update();
    }
    
    // Process queued events at the end of the update cycle
    resolveQueue();
}

void DifficultySelection::render() const {
    sf::RenderWindow &window = Window::getInstance().getRenderWindow();
    
    // Draw overlay background
    window.draw(darkOverlay, sf::RenderStates::Default);
    window.draw(background, sf::RenderStates::Default);
    
    // Draw title
    window.draw(title);
    
    // Draw current difficulty text
    window.draw(currentDifficultyText);
    
    // Draw buttons
    for (const auto &button : buttons) {
        window.draw(*button, sf::RenderStates::Default);
    }
    
    // Draw current difficulty icon if available
    if (currentDifficultyIndex < difficulties.size()) {
        const DifficultyLevel& currentDifficulty = difficulties[currentDifficultyIndex];
        sf::Sprite icon = currentDifficulty.getIcon();
        
        // Position and scale the icon above the difficulty text
        icon.setPosition({background.getPosition().x - 25.f, background.getPosition().y - 40.f});
        icon.setScale({0.8f, 0.8f});
        
        window.draw(icon, sf::RenderStates::Default);
    }
}
