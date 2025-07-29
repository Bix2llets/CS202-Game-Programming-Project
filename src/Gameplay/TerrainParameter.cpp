#include "Gameplay/TerrainParameter.hpp"
#include <utility>
#include <functional>
void TerrainParameter::setSeed(std::string hashStr) {
    std::hash<std::string> hasher;
    this->seed = static_cast<int>(hasher(hashStr));
}
void TerrainParameter::setSeed(int seed) {
    std::hash<std::string> hasher;
    this->seed = seed;
}