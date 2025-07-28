#include "Gameplay/Path.hpp"

#include <cmath>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "Base/Constants.hpp"
#include "Core/ResourceManager.hpp"
#include "Utility/logger.hpp"

const std::vector<sf::Vector2f>* Path::getWaypoints() { return &waypoints; }

void Path::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(*pathSprite, state);
}

void Path::loadWaypoints(const std::vector<sf::Vector2f>& path) {
    waypoints = path;
    sf::RenderTexture mask = getMaskTexture();
    sf::RenderTexture pathComb = getPathTexture();
    pathComb.display();
    mask.display();
    sf::Sprite drawnPath(pathComb.getTexture());
    sf::RenderStates multiState;
    multiState.blendMode = sf::BlendMultiply;

    sf::RenderTexture finalMask;
    if (!finalMask.resize(pathComb.getSize())) {
        Logger::error("Cannot resize finalMask");
    }
    finalMask.clear(sf::Color::Transparent);
    finalMask.draw(drawnPath);
    finalMask.display();
    // * Draw final Mask => wrong
    // * Draw path comb -> nothing
    // * Draw mask => marble pattern

    pathTexture = std::make_unique<sf::Texture>(finalMask.getTexture());
    pathSprite = std::make_unique<sf::Sprite>(*pathTexture);
}

sf::RenderTexture Path::getMaskTexture() const {
    // * Base texture of mask
    const sf::Texture& marbleTexture =
        *ResourceManager::getInstance().getTexture("marble");
    int textureWidth = marbleTexture.getSize().x;
    int textureHeight = marbleTexture.getSize().y;

    static const int width = GameConstants::MAP_WIDTH * GameConstants::CELL_SIZE;
    static const int height = GameConstants::MAP_HEIGHT * GameConstants::CELL_SIZE;
    static const int tileWidth = 32;
    static const int tileHeight = 32;
    sf::RenderTexture mask;
    if (!mask.resize({width, height})) {
        Logger::error("Cannot resize mask");
    };
    mask.clear(sf::Color::Transparent);

    static const int horizontalTiles = textureWidth / tileWidth;
    static const int verticalTiles = textureHeight / tileHeight;

    auto hashGen = [](int val, int MOD) {
        return int(1LL * val * 22071997 % 101 % MOD);
    };
    for (int i = 0; i < width; i += tileWidth)
        for (int j = 0; j < height; j += tileHeight) {
            sf::Vector2i texturePosition = {hashGen(i / 32, horizontalTiles),
                                            hashGen(j / 32, verticalTiles)};
            sf::Vector2i textureSize = {tileWidth, tileHeight};
            sf::Sprite sprite(marbleTexture);
            sprite.setTextureRect({texturePosition * 32, textureSize});
            sprite.setPosition(static_cast<sf::Vector2f>(sf::Vector2i{i, j}));
            // Logger::debug(std::format("{} {}",
            //                           texturePosition.x,
            //                           texturePosition.y));
            mask.draw(sprite);
            // target.draw(sprite, state);
        }
    return std::move(mask);
}

sf::RenderTexture Path::getPathTexture() const {
    sf::RenderTexture pathComb;
    static const int width = GameConstants::MAP_WIDTH * GameConstants::CELL_SIZE;
    static const int height = GameConstants::MAP_HEIGHT * GameConstants::CELL_SIZE;

    if (!pathComb.resize({width, height})) {
        Logger::error("Cannot resize pathComb");
    }
    pathComb.clear(sf::Color::Transparent);
    if (waypoints.size() <= 0) return pathComb;
    sf::VertexArray pathway(sf::PrimitiveType::TriangleStrip,
                            static_cast<int>(waypoints.size() * 4 - 4));
    for (int i = 0; i < waypoints.size(); i++) {
        if (i < waypoints.size() - 1) {
            // Calculate direction vector
            sf::Vector2f dir =
                static_cast<sf::Vector2f>(waypoints[i + 1] - waypoints[i]);
            float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length != 0) dir /= length;

            // Perpendicular vector for thickness
            sf::Vector2f perp(-dir.y, dir.x);

            float thickness = GameConstants::CELL_SIZE *
                              3;  // Set your desired thickness here

            // Offset points
            sf::Vector2f offset = (perp * (thickness / 2.f));
            if (i == 0) {
                pathway[i * 4].position =
                    static_cast<sf::Vector2f>(waypoints[i]) + offset -
                    (dir * thickness) / 2.f;
                pathway[i * 4 + 1].position =
                    static_cast<sf::Vector2f>(waypoints[i]) - offset -
                    (dir * thickness) / 2.f;

            } else {
                pathway[i * 4].position =
                    static_cast<sf::Vector2f>(waypoints[i]) + offset;
                pathway[i * 4 + 1].position =
                    static_cast<sf::Vector2f>(waypoints[i]) - offset;
            }

            if (i == waypoints.size() - 2) {
                pathway[i * 4 + 2].position =
                    (static_cast<sf::Vector2f>(waypoints[i + 1]) + offset +
                     (dir * thickness) / 2.f);
                pathway[i * 4 + 3].position =
                    (static_cast<sf::Vector2f>(waypoints[i + 1]) - offset +
                     (dir * thickness) / 2.f);
            } else {
                pathway[i * 4 + 2].position =
                    static_cast<sf::Vector2f>(waypoints[i + 1]) + offset;
                pathway[i * 4 + 3].position =
                    static_cast<sf::Vector2f>(waypoints[i + 1]) - offset;
            }
        } else {
            // For the last point, repeat the previous offset
            // pathway[i * 2] = (waypoints[i] - waypoints[i -
            // 1]) + pathway[(i - 1) * 2]; pathway[i *
            // 2 + 1] = (waypoints[i] - waypoints[i -
            // 1]) + pathway[(i - 1) * 2 + 1];
        }
    }
    for (int i = 0; i < pathway.getVertexCount(); i++) {
        pathway[i].color = sf::Color::Blue;
        pathway[i].color.a = 100;
    }
    sf::RenderStates state;
    state.blendMode = sf::BlendNone;
    pathComb.draw(pathway, state);
    return std::move(pathComb);
}