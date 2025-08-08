#include <SFML/Graphics.hpp>
#include <optional>

#include "Utility/logger.hpp"
#include "Core/Application.hpp"
#include "Core/ResourceManager.hpp"

int main() {
    freopen("logs.txt", "w", stdout);
    freopen("errors.txt", "w", stderr);
    
    Logger::success("Program start");
    
    Application mainLoop;
    mainLoop.run();
    Logger::success("Program exit success");

    return 0;
}

