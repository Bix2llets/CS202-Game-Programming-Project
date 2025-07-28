#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Gameplay/Height.hpp"
#include "Gameplay/Path.hpp"
#include "Gameplay/PathGenerator.hpp"
#include "Gameplay/TerrainParameter.hpp"
// * Goal for now: Perlin noise generating the height map
class Terrain {
   private:
    static constexpr int TILE_SIZE = 4;
    std::vector<std::vector<Height::Height>> heightMap;
    sf::Texture mapTexture;
    std::unique_ptr<sf::Sprite> map;
    static const std::vector<unsigned int> elevationColors;
    Path path;

   public:
    Terrain();
    Terrain(TerrainParameter parameters);
    Terrain(Terrain&& other) noexcept;
    Terrain& operator=(Terrain&& rhs) noexcept;
    void debugRender();
    void render(sf::RenderStates state) const;
    Height::Height getCellType(sf::Vector2f position);
    Height::Height getCellType(float x, float y);

    const std::vector<sf::Vector2f>* getPath();
};