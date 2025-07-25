#include "Gameplay/terrain.hpp"

#include <math.h>

#include "Core/Window.hpp"
#include "Gameplay/TerrainGenerator.hpp"
#include <fstream>
Terrain::Terrain(int zoomFactor, int octave, float persistance,
                 float lacunarity, long long seed) {
    TerrainGenerator generator;
    generator.setResultSize(sf::Vector2i(800, 800));
    heightMap = generator.getNoiseMap(zoomFactor, octave, persistance,
                                      lacunarity, seed);
    sf::RenderTexture temp;
    bool _ = temp.resize({static_cast<unsigned int>(heightMap.size() * 4),
                          static_cast<unsigned int>(heightMap[0].size() * 4)});
    temp.clear(sf::Color::Transparent);
    auto quantitize = [](float val, int quantizationFreq) {
        if (quantizationFreq <= 0) return val;
        return floor(val * quantizationFreq) / quantizationFreq;
    };

    auto getColor = [](float val) {
        int value = val * 20;
        return sf::Color(elevationColors[value]);
    };
    std::ofstream outFile("heightMap.log");
    for (int y = 0; y < heightMap.size(); y++)
    {

        for (int x = 0; x < heightMap[y].size(); x++) {
            sf::RectangleShape cell;
            outFile << int(20 * heightMap[y][x]) << " ";       
            cell.setFillColor({getColor(heightMap[y][x])});
            // cell.setFillColor({255, 255, 255, 255});
            cell.setPosition({(float)x * 4, (float)y * 4});
            cell.setSize({4.f, 4.f});
            cell.setOrigin({0, 0});
            temp.draw(cell);
        }
        outFile << "\n";
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

const std::vector<unsigned int> Terrain::elevationColors = {
    0x0A2342FF,
    0x0A2342FF,
    0x1A3B66FF,
    0x1A3B66FF,
    0x3E7CB1FF,
    0xF4D35EFF,
    0x5B8C5AFF,
    0x5B8C5AFF,
    0x5B8C5AFF,
    0x5B8C5AFF,
    0x5B8C5AFF,
    0x5B8C5AFF,
    0x8B4513FF,
    0x8B4513FF,
    0x8B4513FF,
    0x6B6B6BFF,
    0x6B6B6BFF,
    0x6B6B6BFF,
    0xE0E0E0FF,
    0xE0E0E0FF,
    0xE0E0E0FF,
    0xE0E0E0FF
};