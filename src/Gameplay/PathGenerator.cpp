#include "Gameplay/PathGenerator.hpp"

#include <fstream>
#include <queue>

#include "Base/Constants.hpp"
#include "Utility/logger.hpp"
PathGenerator::PathGenerator(
    const std::vector<std::vector<Height::Height>> &heightMap)
    : heightMap{heightMap} {
    transitionCost.resize(8);
    transitionCost[Height::DeepSea] =      {000, 400, 300, 200, 300, 400, 450, 500};
    transitionCost[Height::ShallowWater] = {900, 000, 300, 100, 200, 400, 500, 900};
    transitionCost[Height::Sand] =         {900, 550, 000, 300, 450, 500, 550, 900};
    transitionCost[Height::Plain] =        {900, 750, 600, 000, 600, 700, 800, 900};
    transitionCost[Height::Forest] =       {900, 700, 500, 300, 000, 600, 750, 900};
    transitionCost[Height::Rocky] =        {900, 500, 400, 200, 300, 000, 600, 900};
    transitionCost[Height::Mountain] =     {900, 500, 250, 100, 250, 350, 000, 700};
    transitionCost[Height::SnowCap] =      {900, 160, 120, 100, 120, 160, 200, 000};
}

std::vector<sf::Vector2f> PathGenerator::operator()(sf::Vector2i begin,
                                                    sf::Vector2i end) {
    std::ofstream logFile("log.txt");
    static const int INF = INT_MAX;
    std::vector<std::vector<int>> f(
        heightMap.size(), std::vector<int>(heightMap[0].size(), INT_MAX));
    std::vector<std::vector<int>> g(
        heightMap.size(), std::vector<int>(heightMap[0].size(), INT_MAX));
    begin.x /= GameConstants::CELL_SIZE_WIDTH;
    begin.y /= GameConstants::CELL_SIZE_HEIGHT;
    end.x /= GameConstants::CELL_SIZE_WIDTH;
    end.y /= GameConstants::CELL_SIZE_HEIGHT;
    if (heightMap.size() < std::max(begin.x, end.x))
        return std::vector<sf::Vector2f>();
    if (heightMap[0].size() < std::max(begin.y, end.y))
        return std::vector<sf::Vector2f>();

    std::priority_queue<Weight, std::vector<Weight>, WeightCmp> weight;
    static sf::Vector2i direction[4] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
    auto isInBound = [this, begin, end](sf::Vector2i position) {
        // if (position.x < 0) return false;
        // if (position.y < 0) return false;
        if (position.x >= heightMap[0].size()) return false;
        if (position.y >= heightMap.size()) return false;
        if (position.x > end.x) return false;
        if (position.y > end.y) return false;

        return true;
    };

    auto getHeuristic = [end](sf::Vector2i position) {
        const int LENGTH = 10;
        return LENGTH * (abs(position.x - end.x) + abs(position.y - end.y));
    };
    weight.push({begin.x, begin.y, 0, getHeuristic(begin)});
    f[begin.y][begin.x] = 0;
    g[begin.y][begin.x] = 0;
    std::vector<std::vector<sf::Vector2i>> previousCells(
        heightMap.size(),
        std::vector<sf::Vector2i>(heightMap[0].size(), {-1, -1}));

    while (weight.size()) {
        Weight current = weight.top();
        weight.pop();
        if (current.x == end.x && current.y == end.y) {
            break;
        }
        logFile << std::format("Choosing {} {} with f = {}", current.x, current.y, current.distanceFromStart + current.distanceToEnd) << "\n";
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
            if (f[nextCell.y][nextCell.x] > nextCell.distanceFromStart + nextCell.distanceToEnd) canMove = true;
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

                logFile << std::format(
                    "{} {} {} {} {} {}", nextCell.x, nextCell.y,
                    nextCell.distanceFromStart, nextCell.distanceToEnd,
                    transitionCost[heightMap[current.y][current.x]]
                                  [heightMap[nextCell.y][nextCell.x]],
                    heightCost[heightMap[nextCell.y][nextCell.x]]) << "\n";
            }
        }
    }
    int counter = (heightMap.size() + heightMap.size()) * 5;
    sf::Vector2i backtrackPointer = end;
    std::vector<sf::Vector2f> result;
    while (isInBound(backtrackPointer) && counter--) {
        sf::Vector2i mapMarker = {
            backtrackPointer.x * GameConstants::CELL_SIZE_WIDTH,
            backtrackPointer.y * GameConstants::CELL_SIZE_HEIGHT};
        result.push_back(static_cast<sf::Vector2f>(mapMarker));
        backtrackPointer =
            previousCells[backtrackPointer.y][backtrackPointer.x];
    }
    std::reverse(result.begin(), result.end());
    for (auto &node : result) {
        Logger::debug(std::format("{} {}", node.x, node.y));
    }

    for (auto x : f) {
        for (auto y : x) logFile << y << " ";
        logFile << "\n";
    }
    logFile << "\n";
    return result;
}