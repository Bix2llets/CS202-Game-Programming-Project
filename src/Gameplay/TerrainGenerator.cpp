#include "Gameplay/TerrainGenerator.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <thread>
#include <random>

#include "Utility/logger.hpp"
TerrainGenerator::TerrainGenerator() {};

std::vector<std::vector<float>> TerrainGenerator::getNoiseMap(
    int zoomFactor, int octaves, float persistence, float lacunarity, long long seed) {

    if (zoomFactor == 0) zoomFactor = 1;
    createPermutation(seed);
    std::vector<std::vector<float>> result;
    result.resize(resultSize.y);
    for (auto &vect : result) vect.resize(resultSize.x);

    float initialFrequency = 1.f / zoomFactor;
    float maxAmplitude = 0.0f;
    float currentAmplitude = 1.0f;
    for (int o = 0; o < octaves; ++o) {
        maxAmplitude += currentAmplitude;
        currentAmplitude *= persistence;
    }
    // -----------
    // * Concurrency generating the terrain
    // * Use Threads
    // ------------

    int available = std::thread::hardware_concurrency();
    if (available == 0) available = 1;
    std::vector<std::thread> threads;
    threads.reserve(available);
    int rowsPerThread = resultSize.y / available;
    int leftover = resultSize.y - rowsPerThread * available;
    Logger::debug("Begin multithreading");
    for (int rowIndex = 0; rowIndex < available; rowIndex++) {
        int startY = rowIndex * rowsPerThread;
        int endY = startY + rowsPerThread - 1;
        if (rowIndex == available - 1) endY += leftover;

        Logger::debug("Add threads");
        threads.emplace_back([this, startY, endY, initialFrequency, persistence,
                              lacunarity, maxAmplitude, octaves, &result]() {
            for (int y = startY; y <= endY; y++) {
                for (int x = 0; x < resultSize.x; x++) {
                    float amplitude = 1.0f;
                    float noiseHeight = 0.0f;
                    float currentFrequency = initialFrequency;
                    for (int o = 0; o < octaves; ++o) {
                        int currentGridSize =
                            std::max(1, static_cast<int>(1 / currentFrequency));
                        float sampleX = x * currentFrequency;
                        float sampleY = y * currentFrequency;
                        float perlinValue = perlin(sampleX, sampleY);

                        noiseHeight += perlinValue * amplitude;
                        amplitude *= persistence;
                        currentFrequency *= lacunarity;
                    }

                    result[y][x] = (noiseHeight / maxAmplitude + 1.0f) / 2.0f;

                    result[y][x] = std::max(0.0f, std::min(1.0f, result[y][x]));
                }
            }
        });
    }

    for (auto &thread : threads) {
        if (thread.joinable()) thread.join();
    }
    return result;
}

// --- Perlin Noise Implementation (Modified to avoid 256-pixel repetition) ---
float TerrainGenerator::perlin(float x, float y) {

    int xGrid = static_cast<int>(std::floor(x));
    int yGrid = static_cast<int>(std::floor(y));

    float xf = x - xGrid;
    float yf = y - yGrid;

    float u = fade(xf);
    float v = fade(yf);

    int upLeftHash = permutation[hashCoordinates(xGrid, yGrid)];
    int upRightHash = permutation[hashCoordinates(xGrid + 1, yGrid)];
    int bottomLeftHash = permutation[hashCoordinates(xGrid, yGrid + 1)];
    int bottomRightHash = permutation[hashCoordinates(xGrid + 1, yGrid + 1)];

    float grad_upLeft =
        getGradient(upLeftHash).x * xf + getGradient(upLeftHash).y * yf;
    float grad_upRight = getGradient(upRightHash).x * (xf - 1.0f) +
                         getGradient(upRightHash).y * yf;
    float grad_bottomLeft = getGradient(bottomLeftHash).x * xf +
                            getGradient(bottomLeftHash).y * (yf - 1.0f);
    float grad_bottomRight = getGradient(bottomRightHash).x * (xf - 1.0f) +
                             getGradient(bottomRightHash).y * (yf - 1.0f);

    float ix1 = lerp(grad_upLeft, grad_upRight, u);
    float ix2 = lerp(grad_bottomLeft, grad_bottomRight, u);
    float result = lerp(ix1, ix2, v);

    return result;
}

sf::Vector2f TerrainGenerator::getGradient(int hash) {
    switch (hash & 7) {
        case 0:
            return sf::Vector2f(1, 1);
        case 1:
            return sf::Vector2f(-1, 1);
        case 2:
            return sf::Vector2f(1, -1);
        case 3:
            return sf::Vector2f(-1, -1);
        case 4:
            return sf::Vector2f(1, 0);
        case 5:
            return sf::Vector2f(-1, 0);
        case 6:
            return sf::Vector2f(0, 1);
        case 7:
            return sf::Vector2f(0, -1);
    }
    return sf::Vector2f(0, 0);
}

int TerrainGenerator::hashCoordinates(int xGrid, int yGrid) {
    return permutation
        [(permutation[(permutation[xGrid & 255] + (yGrid & 255)) & 255]) & 255];
}

float TerrainGenerator::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);  // 6t^5 - 15t^4 + 10t^3
}

float TerrainGenerator::lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void TerrainGenerator::setResultSize(sf::Vector2i size) { resultSize = size; }

void TerrainGenerator::createPermutation(long long seed) {
    std::vector<int> shuffleArr(256);
    for (int i = 0; i < shuffleArr.size(); i++)
        shuffleArr[i] = i;
    
    std::mt19937 rd(seed);
    std::shuffle(shuffleArr.begin(), shuffleArr.end(), rd);

    permutation = shuffleArr;
    permutation.insert(permutation.end(), shuffleArr.begin(), shuffleArr.end());
}