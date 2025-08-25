#pragma once

#include "Scene/Overlays/Overlay.hpp"
#include "Gameplay/Difficulty.hpp"
#include <vector>

class DifficultySelection : public Overlay {
private:
    std::vector<DifficultyLevel> difficulties;
    sf::Text currentDifficultyText;
    std::string selectedLevelId;
    size_t currentDifficultyIndex;
    
    void constructButtons();
    void loadDifficulties();
    void updateDifficultyDisplay();
    void cycleToPrevious();
    void cycleToNext();

public:
    DifficultySelection(Mediator& mediator, const std::string& levelId);
    ~DifficultySelection();

    void update() override;
    void render() const override;

    bool onMouseEvent(Mouse mouse, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override {
        return false; // No scroll event handling in difficulty selection
    }
};