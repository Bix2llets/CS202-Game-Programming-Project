#include "Gameplay/terrain.hpp"

#include <math.h>

#include <fstream>

#include "Core/Window.hpp"
#include "Gameplay/TerrainGenerator.hpp"
#include "Utility/logger.hpp"
#include "Base/Constants.hpp"

#include "Gameplay/Waypoint.hpp"
Terrain::Terrain(TerrainParameters parameter) {
    TerrainGenerator generator;
    generator.setResultSize(sf::Vector2i(GameConstants::MAP_WIDTH, GameConstants::MAP_HEIGHT));
    std::vector<std::vector<float>> perlinResult = generator.getNoiseMap(parameter);

    sf::RenderTexture temp;
    bool _ =
        temp.resize({static_cast<unsigned int>(perlinResult.size() * GameConstants::CELL_SIZE),
                     static_cast<unsigned int>(perlinResult[0].size() * GameConstants::CELL_SIZE)});
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
            cell.setPosition(sf::Vector2f{(float)x, (float)y} * static_cast<float>(GameConstants::CELL_SIZE));
            cell.setSize({GameConstants::CELL_SIZE, GameConstants::CELL_SIZE});
            cell.setOrigin({0, 0});
            temp.draw(cell);
        }
    }
    temp.display();

    mapTexture = std::move(temp.getTexture());
    map = std::make_unique<sf::Sprite>(mapTexture);

    PathGenerator AStarPath(heightMap);
    path.loadWaypoints(AStarPath(sf::Vector2i{0, 0}, sf::Vector2i{GameConstants::MAP_WIDTH, GameConstants::MAP_HEIGHT} * GameConstants::CELL_SIZE));
}

void Terrain::debugRender() {
    Window::getInstance().getRenderWindow().draw(*map);
    Window::getInstance().getRenderWindow().draw(path);
}

Terrain::Terrain() : Terrain(TerrainParameters()) {}

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
    if (position.x < 0) return Height::DeepSea;
    if (position.y < 0) return Height::DeepSea;
    if (position.x / GameConstants::CELL_SIZE >= heightMap.size()) return Height::DeepSea;
    if (position.y / GameConstants::CELL_SIZE >= heightMap[0].size()) return Height::DeepSea;
    return heightMap[static_cast<int>(position.y) / GameConstants::CELL_SIZE]
                    [static_cast<int>(position.x) / GameConstants::CELL_SIZE];
}

const std::vector<Waypoint>* Terrain::getPath() {
    return &path.getWaypoints();
}

void Terrain::render(sf::RenderStates state) const {
    Window::getInstance().getRenderWindow().draw(*map, state);
    Window::getInstance().getRenderWindow().draw(path, state);
}