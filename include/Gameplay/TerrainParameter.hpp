#pragma once

#include <string>
struct TerrainParameters {
    int seed = 22071997;
    float gridSize = 150;
    int octaves = 4;
    float persistence = 0.5f;
    float lacunarity = 2.f;
    float depthFactor = 1.f;

    void setSeed(std::string hashStr);
    void setSeed(int hash);
};