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
    std::vector<std::vector<float>> perlinResult = generator.getNoiseMap(
        zoomFactor, octave, persistance, lacunarity, seed, depthFactor);

    // std::vector<std::vector<float>> perlinResult(
    //     100, std::vector<float>(100, 0.55f));
    // for (int i = 5; i <= 8; i++) {
    //     perlinResult[i] = std::vector<float>(100, 0.f);
    //     for (int j = perlinResult[i].size() - 1; j >= perlinResult[i].size()
    //     - 5; j--)
    //         perlinResult[i][j] = 0.55f;
    // }
    // for (int i = 55; i <= 68; i++) {
    //     perlinResult[i] = std::vector<float>(100, 0.f);
    //     for (int j = 9; j <= 40; j++)
    //         perlinResult[i][j] = 0.55f;
    // }

    sf::RenderTexture temp;
    bool _ =
        temp.resize({static_cast<unsigned int>(perlinResult.size() * 4),
                     static_cast<unsigned int>(perlinResult[0].size() * 4)});
    heightMap.resize(perlinResult.size());
    for (int i = 0; i < heightMap.size(); i++)
        heightMap[i].resize(perlinResult.size());
    temp.clear(sf::Color::Transparent);
    auto quantitize = [](float val) {
        // * Quantitize height based on the noise value
        if (val > 0.89f) return Height::SnowCap;   // * Hgh mountain
        if (val > 0.77f) return Height::Mountain;  // * Med mountain
        if (val > 0.65f) return Height::Rocky;     // * Low mountain
        // * Mountain area

        if (val > 0.55f) return Height::Forest;        // * Dark plain
        if (val > 0.40f) return Height::Plain;         // * Light plain
        if (val > 0.25f) return Height::Sand;          // * Sand
        if (val > 0.15f) return Height::ShallowWater;  // * Shallow sea
        // Logger::debug(std::format("{}", val));
        return Height::DeepSea;  // * Deep sea
    };

    auto getColor = [quantitize](float val) {
        return sf::Color(elevationColors[val]);
    };
    for (int y = 0; y < heightMap.size(); y++) {
        for (int x = 0; x < heightMap[y].size(); x++) {
            sf::RectangleShape cell;
            heightMap[y][x] = quantitize(perlinResult[y][x]);
            cell.setFillColor({getColor(heightMap[y][x])});
            // cell.setFillColor({255, 255, 255, 255});
            cell.setPosition({(float)x * 4, (float)y * 4});
            cell.setSize({4.f, 4.f});
            cell.setOrigin({0, 0});
            temp.draw(cell);
        }
    }
    temp.display();

    mapTexture = std::move(temp.getTexture());
    map = std::make_unique<sf::Sprite>(mapTexture);

    PathGenerator AStarPath(heightMap);
    path.loadWaypoints(AStarPath(sf::Vector2i{0, 0}, sf::Vector2i{1000, 800}));
    // path.loadWaypoints(AStarPath(sf::Vector2i{0, 0}, sf::Vector2i{400,
    // 400}));
}

void Terrain::debugRender() {
    Window::getInstance().getRenderWindow().draw(*map);
    Window::getInstance().getRenderWindow().draw(path);
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
    path = std::move(other.path);
    other.mapTexture = sf::Texture();
}

Terrain& Terrain::operator=(Terrain&& rhs) noexcept {
    if (this == &rhs) return *this;
    mapTexture = std::move(rhs.mapTexture);
    heightMap = std::move(rhs.heightMap);
    map = std::move(rhs.map);
    map = std::make_unique<sf::Sprite>(mapTexture);
    path = std::move(rhs.path);
    rhs.mapTexture = sf::Texture();
    return *this;
}

const std::vector<unsigned int> Terrain::elevationColors = {
    0x00478aff, 0x0066CCFF, 0xfce490ff, 0x5C943CFF,
    0x266400ff, 0x9F8D8DFF, 0x4E4C4FFF, 0xFFFFFFFF};

Height::Height Terrain::getCellType(sf::Vector2f position) {
    return heightMap[static_cast<int>(position.y) / 4]
                    [static_cast<int>(position.x) / 4];
}