#include "Gameplay/TerrainParameter.hpp"
#include <utility>
#include <functional>
void TerrainParameters::setSeed(std::string hashStr) {
    std::hash<std::string> hasher;
    this->seed = static_cast<int>(hasher(hashStr));
}
void TerrainParameters::setSeed(int seed) {
    std::hash<std::string> hasher;
    this->seed = seed;
}