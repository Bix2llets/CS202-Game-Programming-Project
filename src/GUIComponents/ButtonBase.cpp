#include "GUIComponents/ButtonBase.hpp"

#include "Core/InputManager.hpp"
#include "Core/MouseState.hpp"
#include "Core/ResourceManager.hpp"

#include "Core/JSONLoader.hpp"
ButtonBase::ButtonBase() {}

ButtonBase::~ButtonBase() {

}

void ButtonBase::update() { graphicState.update(); }
