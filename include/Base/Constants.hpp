/**
 * @file Constants.hpp
 * @brief Contains global game constants such as window size, title, and timing intervals.
 */
#pragma once
/**
 * @namespace GameConstants
 * @brief Namespace for storing game-wide constant values.
 */
namespace GameConstants {
    constexpr int DEFAULT_WINDOW_WIDTH = 1200;
    constexpr int DEFAULT_WINDOW_HEIGHT = 900;

    constexpr int WINDOW_WIDTH_1 = 1024;
    constexpr int WINDOW_HEIGHT_1 = 768;
    
    constexpr int WINDOW_WIDTH_2 = 1440;
    constexpr int WINDOW_HEIGHT_2 = 1080;
    
    constexpr const char* WINDOW_TITLE = "Rampart Remains";

    constexpr float TICK_INTERVAL = 1.f / 60;
    constexpr float SUBTICK_INTERVAL = 1.f / 60 / 8;
    constexpr int TARGET_FPS = 60;
}