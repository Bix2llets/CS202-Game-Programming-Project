#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Gameplay/Height.hpp"
#include "Gameplay/Path.hpp"
#include "Gameplay/PathGenerator.hpp"
// * Goal for now: Perlin noise generating the height map
class Terrain{
    private:
    static constexpr int TILE_SIZE = 4;
    std::vector<std::vector<Height::Height>> heightMap;
    sf::Texture mapTexture;
    std::unique_ptr<sf::Sprite> map;
    static const std::vector<unsigned int> elevationColors; 
    Path path;
    
    public:
    Terrain();
    Terrain(int gridSize, int octaves, float persistance, float lacunarity, long long seed, float depthFactor);
    Terrain(Terrain&& other) noexcept;
    Terrain& operator=(Terrain&& rhs) noexcept;
    void debugRender();

    Height::Height getCellType(sf::Vector2f position);
    Height::Height getCellType(float x, float y);
};