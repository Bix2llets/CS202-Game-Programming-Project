#include "GUIComponents/ButtonBase.hpp"

#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"

#include "Core/JSONLoader.hpp"
ButtonBase::ButtonBase() {}

ButtonBase::~ButtonBase() {
    // Destructor implementation if needed
}

void ButtonBase::update() { graphicState.update(); }
