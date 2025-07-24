#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
// * Goal for now: Perlin noise generating the height map
class Terrain{
    public:
    static constexpr int TILE_SIZE = 4;
    std::vector<std::vector<float>> heightMap;
    sf::Texture mapTexture;
    std::unique_ptr<sf::Sprite> map;

    Terrain();
    Terrain(int gridSize, int octaves, float persistance, float lacunarity, long long seed);
    Terrain(Terrain&& other) noexcept;
    Terrain& operator=(Terrain&& rhs) noexcept;
    void debugRender();
};