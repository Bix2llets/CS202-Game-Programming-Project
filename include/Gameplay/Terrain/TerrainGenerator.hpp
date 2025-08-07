#pragma once

#include <array>
#include <vector>
#include <SFML/System/Vector2.hpp> 

#include "Gameplay/Terrain/TerrainParameter.hpp"
class TerrainGenerator {
public:
    std::vector<int> permutation;

    std::vector<std::vector<float>> getNoiseMap(TerrainParameters parameter);

    void setResultSize(sf::Vector2i size);

    TerrainGenerator();

private:
    sf::Vector2i resultSize;

    float perlin(float x, float y);
    float fade(float t);
    float lerp(float a, float b, float t);
    sf::Vector2f getGradient(int hash); 

    int hashCoordinates(int xGrid, int yGrid);

    void createPermutation(int seed);
};