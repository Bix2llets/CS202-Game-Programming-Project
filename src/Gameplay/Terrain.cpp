#include "Gameplay/terrain.hpp"

#include <math.h>

#include <fstream>

#include "Core/Window.hpp"
#include "Gameplay/TerrainGenerator.hpp"
#include "Utility/logger.hpp"
Terrain::Terrain(int zoomFactor, int octave, float persistance,
                 float lacunarity, long long seed, float depthFactor) {
    TerrainGenerator generator;
    generator.setResultSize(sf::Vector2i(800, 800));
    heightMap = generator.getNoiseMap(zoomFactor, octave, persistance,
                                      lacunarity, seed, depthFactor);
    sf::RenderTexture temp;
    bool _ = temp.resize({static_cast<unsigned int>(heightMap.size() * 4),
                          static_cast<unsigned int>(heightMap[0].size() * 4)});
    temp.clear(sf::Color::Transparent);
    auto quantitize = [](float val) {
        // * Quantitize height based on the noise value
        if (val > 0.89f) return 7;  // * High mountain
        if (val > 0.77f) return 6;  // * Med mountain
        if (val > 0.65f) return 5;   // * Low mountain
        // * Mountain area

        if (val > 0.55f) return 4;  // * Dark plain
        if (val > 0.40f) return 3;  // * Light plain
        if (val > 0.25f) return 2;  // * Sand
        if (val > 0.15f) return 1;  // * Shallow sea
        // Logger::debug(std::format("{}", val));
        return 0;  // * Deep sea
    };

    auto getColor = [quantitize](float val) {
        return sf::Color(elevationColors[quantitize(val)]);
    };
    std::ofstream outFile("heightMap.log");
    for (int y = 0; y < heightMap.size(); y++) {
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

Terrain::Terrain() : Terrain(50, 4, 0.5, 20, 0, 1) {}

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
    0x00478aff, 0x0066CCFF, 0xfce490ff, 0x5C943CFF,
    0x266400ff, 0x9F8D8DFF, 0x4E4C4FFF, 0xFFFFFFFF};