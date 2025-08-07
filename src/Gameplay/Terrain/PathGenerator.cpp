#include "Gameplay/Terrain/PathGenerator.hpp"

#include <fstream>
#include <queue>

#include "Base/Constants.hpp"
#include "Gameplay/Waypoint.hpp"
#include "Utility/logger.hpp"
PathGenerator::PathGenerator(
    const std::vector<std::vector<Height::Height>> &heightMap)
    : heightMap{heightMap} {
    transitionCost.resize(8);
    transitionCost[Height::DeepSea] = {000, 300, 300, 200, 300, 400, 450, 500};
    transitionCost[Height::ShallowWater] = {200, 000, 300, 100,
                                            200, 400, 500, 900};
    transitionCost[Height::Sand] = {900, 400, 000, 300, 450, 500, 550, 900};
    transitionCost[Height::Plain] = {900, 750, 100, 000, 100, 700, 800, 900};
    transitionCost[Height::Forest] = {900, 700, 500, 100, 000, 200, 750, 900};
    transitionCost[Height::Rocky] = {900, 500, 400, 200, 300, 000, 300, 900};
    transitionCost[Height::Mountain] = {900, 500, 250, 100, 250, 350, 000, 400};
    transitionCost[Height::SnowCap] = {900, 160, 120, 100, 120, 160, 200, 000};
}

std::vector<Waypoint> PathGenerator::operator()(sf::Vector2i begin,
                                                sf::Vector2i end) {
    Logger::debug("Begin path generation");
    // std::ofstream logFile("log.txt");
    static const int INF = INT_MAX;
    std::vector<std::vector<int>> f(
        heightMap.size(), std::vector<int>(heightMap[0].size(), INT_MAX));
    std::vector<std::vector<int>> g(
        heightMap.size(), std::vector<int>(heightMap[0].size(), INT_MAX));
    begin.x /= GameConstants::CELL_SIZE;
    begin.y /= GameConstants::CELL_SIZE;
    end.x /= GameConstants::CELL_SIZE;
    end.y /= GameConstants::CELL_SIZE;
    static const int SOFT_BOUND = 5;
    std::priority_queue<Weight, std::vector<Weight>, WeightCmp> weight;
    static sf::Vector2i direction[4] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
    auto isInBound = [this, begin, end](sf::Vector2i position) {
        if (position.x < 0 + SOFT_BOUND) return false;
        if (position.y < 0 + SOFT_BOUND) return false;
        if (position.x > heightMap[0].size() - SOFT_BOUND) return false;
        if (position.y > heightMap.size() - SOFT_BOUND) return false;
        // if (position.x > end.x) return false;
        // if (position.y > end.y) return false;

        return true;
    };

    auto getHeuristic = [end](sf::Vector2i position) {
        const int LENGTH = 10;
        return LENGTH * (abs(position.x - end.x) + abs(position.y - end.y));
    };

    auto clampBound = [this](sf::Vector2i &value) {
        value.x =
            std::max((int)SOFT_BOUND,
                     std::min(value.x, (int)heightMap[0].size() - SOFT_BOUND));
        value.y =
            std::max((int)SOFT_BOUND,
                     std::min(value.y, (int)heightMap.size() - SOFT_BOUND));
    };
    clampBound(begin);
    clampBound(end);
    weight.push({begin.x, begin.y, 0, getHeuristic(begin)});
    f[begin.y][begin.x] = 0;
    g[begin.y][begin.x] = 0;
    std::vector<std::vector<sf::Vector2i>> previousCells(
        heightMap.size(),
        std::vector<sf::Vector2i>(heightMap[0].size(), {-1, -1}));
    Logger::debug(std::format("Begin A-Star searching from {} {} to {} {}",
                              begin.x, begin.y, end.x, end.y));
    Logger::debug(std::format("The height map size is {} {}", heightMap.size(),
                              heightMap[0].size()));
    while (weight.size()) {
        Weight current = weight.top();
        weight.pop();
        if (current.x == end.x && current.y == end.y) {
            break;
        }
        // logFile << std::format("Searching at {} {} with g = {}, h = {}\n",
        // current.x, current.y, current.distanceFromStart,
        // current.distanceToEnd); logFile << std::format("Choosing {} {} with f
        // = {}", current.x, current.y, current.distanceFromStart +
        // current.distanceToEnd) <<
        // "\n";
        for (int i = 0; i < 4; i++) {
            Weight nextCell;
            nextCell.x = current.x + direction[i].x;
            nextCell.y = current.y + direction[i].y;
            if (!isInBound({nextCell.x, nextCell.y})) continue;
            nextCell.distanceFromStart =
                current.distanceFromStart +
                heightCost[heightMap[current.y][current.x]] +
                transitionCost[heightMap[current.y][current.x]]
                              [heightMap[nextCell.y][nextCell.x]];
            nextCell.distanceToEnd = getHeuristic({nextCell.x, nextCell.y});
            bool canMove = false;
            if (f[nextCell.y][nextCell.x] >
                nextCell.distanceFromStart + nextCell.distanceToEnd)
                canMove = true;
            // if (nextCell.distanceFromStart + nextCell.distanceToEnd ==
            //     f[nextCell.y][nextCell.x])
            //     if (nextCell.distanceFromStart < g[nextCell.y][nextCell.x])
            //         canMove = true;
            if (canMove) {
                f[nextCell.y][nextCell.x] =
                    nextCell.distanceFromStart + nextCell.distanceToEnd;
                g[nextCell.y][nextCell.x] = nextCell.distanceFromStart;
                previousCells[nextCell.y][nextCell.x] = {current.x, current.y};
                weight.push(nextCell);

                // logFile << std::format(
                //     "{} {} {} {} {} {}", nextCell.x, nextCell.y,
                //     nextCell.distanceFromStart, nextCell.distanceToEnd,
                //     transitionCost[heightMap[current.y][current.x]]
                //                   [heightMap[nextCell.y][nextCell.x]],
                //     heightCost[heightMap[nextCell.y][nextCell.x]]) << "\n";
            }
        }
    }

    auto getSpeedMultiplier = [](Height::Height cellHeight) {
        switch (cellHeight) {
            case Height::DeepSea:
                return 0.5;
            case Height::ShallowWater:
                return 0.75;
            case Height::Sand:
                return 0.9;
            case Height::Plain:
                return 1.0;
            case Height::Forest:
                return 0.8;
            case Height::Rocky:
                return 0.7;
            case Height::Mountain:
                return 0.6;
            case Height::SnowCap:
                return 0.5;
            default:
                return 1.0;
        }
    };
    Logger::debug("Begin tracing");
    int counter = (heightMap.size() + heightMap.size()) * 5;
    sf::Vector2i backtrackPointer = end;
    std::vector<Waypoint> result;
    while (isInBound(backtrackPointer) && counter--) {
        sf::Vector2i mapMarker = {
            backtrackPointer.x * GameConstants::CELL_SIZE,
            backtrackPointer.y * GameConstants::CELL_SIZE};
        result.emplace_back(
            Waypoint{{mapMarker.x, mapMarker.y},
                     getSpeedMultiplier(
                         heightMap[backtrackPointer.y][backtrackPointer.x])});
        backtrackPointer =
            previousCells[backtrackPointer.y][backtrackPointer.x];
    }
    std::reverse(result.begin(), result.end());
    // for (auto &node : result) {
    //     Logger::debug(std::format("{} {} {}", node.position.x,
    //     node.position.y, node.speedMultiplier));
    // }

    // for (auto x : f) {
    //     for (auto y : x) logFile << y << " ";
    //     logFile << "\n";
    // }
    // logFile << "\n";
    return result;
}