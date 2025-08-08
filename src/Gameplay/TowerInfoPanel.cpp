#include "Gameplay/TowerInfoPanel.hpp"

TowerInfoPanel& TowerInfoPanel::getInstance() {
    static TowerInfoPanel instance;
    return instance;
}

void TowerInfoPanel::render(sf::RenderStates state) {
    if (!referencingTower) return;

    // Render the tower information panel
    // This is a placeholder for actual rendering logic
    // You can use sf::Text, sf::Sprite, etc. to display tower information
}

void TowerInfoPanel::setFocus(Tower* tower) {
    referencingTower = tower;
    // Additional logic to update the panel with tower information
}

void TowerInfoPanel::deFocus() {
    referencingTower = nullptr;
    // Additional logic to clear the panel
}

void TowerInfoPanel::update() {
    if (!referencingTower) return;

    // Update the panel with the current state of the tower
    // This could include updating text, sprites, etc.
}

