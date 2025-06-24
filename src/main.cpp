#include <SFML/Graphics.hpp>
#include <optional>

#include "Utility/logger.hpp"
#include "Core/Application.hpp"
#include "Core/ResourceManager.hpp"
int main() {
    Logger::success("Program start");
    


    
    Application mainLoop;
    mainLoop.run();
    
    Logger::success("Program exit success");
}

