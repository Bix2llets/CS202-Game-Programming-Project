#include "Gameplay/Path.hpp"

#include <cmath>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "Core/ResourceManager.hpp"
#include "Utility/logger.hpp"
#include "Base/Constants.hpp"

const std::vector<Waypoint>* Path::getWaypoints(int pathNumber) {
    return &mapWaypoints[pathNumber];
}

void Path::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    sf::RenderTexture mask = getMaskTexture();
    sf::RenderTexture pathComb = getPathTexture();
    pathComb.display();
    sf::Sprite path(pathComb.getTexture());
    sf::RenderStates multiState = state;
    multiState.blendMode = sf::BlendMultiply;
    mask.draw(path, multiState);
    mask.display();
    sf::Sprite maskedPath(mask.getTexture());
    target.draw(maskedPath, state);
}

void Path::loadWaypoints(const std::vector<Waypoint>& path, int pathID) {
    if (mapWaypoints.size() <
        pathID + 1)  //> Accomodate for the starting  index of 0
        mapWaypoints.resize(pathID + 1);

    mapWaypoints[pathID] = path;
}

sf::RenderTexture Path::getMaskTexture() const {
    // * Base texture of mask
    const sf::Texture& marbleTexture =
        *ResourceManager::getInstance().getTexture("marble");
    int textureWidth = marbleTexture.getSize().x;
    int textureHeight = marbleTexture.getSize().y;

    static const int width =  GameConstants::RENDER_TEXTURE_WIDTH;
    static const int height = GameConstants::RENDER_TEXTURE_HEIGHT;
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
    static const int width =  GameConstants::RENDER_TEXTURE_WIDTH;
    static const int height = GameConstants::RENDER_TEXTURE_HEIGHT;

    if (!pathComb.resize({width, height})) {
        Logger::error("Cannot resize pathComb");
    }
    pathComb.clear(sf::Color::Transparent);
    for (const std::vector<Waypoint> path : mapWaypoints) {
        sf::VertexArray pathway(sf::PrimitiveType::TriangleStrip,
                                static_cast<int>(path.size() * 4 - 4));
        for (int i = 0; i < path.size(); i++) {
            if (i < path.size() - 1) {
                // Calculate direction vector
                sf::Vector2f dir = path[i + 1].position - path[i].position;
                float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (length != 0) dir /= length;

                // Perpendicular vector for thickness
                sf::Vector2f perp(-dir.y, dir.x);

                float thickness = 45.f;  // Set your desired thickness here

                // Offset points
                sf::Vector2f offset = (perp * (thickness / 2.f));
                if (i == 0) {
                    pathway[i * 4].position =
                        path[i].position + offset - (dir * thickness) / 2.f;
                    pathway[i * 4 + 1].position =
                        path[i].position - offset - (dir * thickness) / 2.f;

                } else {
                    pathway[i * 4].position = path[i].position + offset;
                    pathway[i * 4 + 1].position = path[i].position - offset;
                }

                if (i == path.size() - 2) {
                    pathway[i * 4 + 2].position =
                        path[i + 1].position + offset + (dir * thickness) / 2.f;
                    pathway[i * 4 + 3].position =
                        path[i + 1].position - offset + (dir * thickness) / 2.f;
                } else {
                    pathway[i * 4 + 2].position = path[i + 1].position + offset;
                    pathway[i * 4 + 3].position = path[i + 1].position - offset;
                }
            } else {
                // For the last point, repeat the previous offset
                // pathway[i * 2].position = (path[i].position - path[i -
                // 1].position) + pathway[(i - 1) * 2].position; pathway[i *
                // 2 + 1].position = (path[i].position - path[i -
                // 1].position) + pathway[(i - 1) * 2 + 1].position;
            }
        }
        for (int i = 0; i < pathway.getVertexCount(); i++)
            pathway[i].color = sf::Color::White;

        if (path.size() > 1) {
            pathComb.draw(pathway);
        }
    }
    return std::move(pathComb);
}