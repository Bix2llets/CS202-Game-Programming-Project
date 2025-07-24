#include "Gameplay/terrain.hpp"

#include <math.h>

#include "Core/Window.hpp"
#include "Gameplay/TerrainGenerator.hpp"
Terrain::Terrain(int zoomFactor, int octave, float persistance, float lacunarity,
                 long long seed) {
    TerrainGenerator generator;
    generator.setResultSize(sf::Vector2i(800, 800));
    heightMap =
        generator.getNoiseMap(zoomFactor, octave, persistance, lacunarity, seed);
    sf::RenderTexture temp;
    temp.resize({heightMap.size() * 4, heightMap[0].size() * 4});
    temp.clear(sf::Color::Transparent);
    auto quantitize = [](float val, int quantizationFreq) {
        if (quantizationFreq <= 0) return val;
        return floor(val * quantizationFreq) / quantizationFreq;
    };
    for (int y = 0; y < heightMap.size(); y++)
        for (int x = 0; x < heightMap[y].size(); x++) {
            sf::RectangleShape cell;

            cell.setFillColor(
                {255, 255, 255,
                 (unsigned char)(quantitize(heightMap[y][x], 0) * 255)});
            // cell.setFillColor({255, 255, 255, 255});
            cell.setPosition({(float)x * 4, (float)y * 4});
            cell.setSize({4.f, 4.f});
            cell.setOrigin({0, 0});
            temp.draw(cell);
        }
    temp.display();
    
    mapTexture = std::move(temp.getTexture());
    map = std::make_unique<sf::Sprite>(mapTexture);
}

void Terrain::debugRender() {
    Window::getInstance().draw(*map);
    // for (int y = 0; y < heightMap.size(); y++)
    //     for (int x = 0; x < heightMap[y].size(); x++) {
    //         sf::RectangleShape cell;
    //         cell.setFillColor(
    //             {(unsigned char)(heightMap[y][x] * 255), 255, 255, 255});
    //         // cell.setFillColor({255, 255, 255, 255});
    //         cell.setPosition({(float)x * 32, (float)y * 32});
    //         cell.setSize({32.f, 32.f});
    //         cell.setOrigin({0, 0});
    //         Window::getInstance().draw(cell);
    //     }
}

Terrain::Terrain() : Terrain(50, 4, 0.5, 20, 0) {}

Terrain::Terrain(Terrain&& other) noexcept
    : mapTexture(std::move(other.mapTexture)),
      heightMap(std::move(other.heightMap)),
      map(std::move(other.map)) {
    map = std::make_unique<sf::Sprite>(mapTexture);
    other.mapTexture = sf::Texture();
}

Terrain& Terrain::operator=(Terrain&& rhs) noexcept {
    if (this == &rhs) return *this;
    mapTexture = std::move(rhs.mapTexture);
    heightMap = std::move(rhs.heightMap);
    map = std::move(rhs.map);
    map = std::make_unique<sf::Sprite>(mapTexture);
    rhs.mapTexture = sf::Texture();
    return *this;
}