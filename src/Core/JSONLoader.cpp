
#include "Core/JSONLoader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// Helper: loads all JSON objects from a file (single object or array of
// objects)
static void loadJsonObjectsFromFile(
    const std::string& filePath,
    std::unordered_map<std::string, nlohmann::json>& outMap) {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open JSON file: " << filePath << std::endl;
        return;
    }
    nlohmann::json j;
    try {
        inFile >> j;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error in " << filePath << ": " << e.what()
                  << std::endl;
        return;
    }
    if (j.is_array()) {
        for (const auto& obj : j) {
            if (obj.contains("id") && obj["id"].is_string()) {
                outMap[obj["id"].get<std::string>()] = obj;
            }
        }
    } else if (j.is_object()) {
        if (j.contains("id") && j["id"].is_string()) {
            outMap[j["id"].get<std::string>()] = j;
        }
    }
}

void JSONLoader::loadFromDirectory(
    const std::string& directory,
    std::unordered_map<std::string, nlohmann::json>& outMap) {
    if (!fs::exists(directory) || !fs::is_directory(directory)) return;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            loadJsonObjectsFromFile(entry.path().string(), outMap);
        }
    }
}

void JSONLoader::loadAll() {
    enemies.clear();
    levels.clear();
    projectiles.clear();
    towers.clear();
    textures.clear();
    fonts.clear();
    sounds.clear();
    musics.clear();
    styles.clear();

    const std::vector<std::string> roots = {"content", "mod"};
    const std::vector<std::pair<
        std::string, std::unordered_map<std::string, nlohmann::json>*>>
        types = {{"enemy", &enemies},
                 {"level", &levels},
                 {"projectile", &projectiles},
                 {"tower", &towers},
                 {"texture", &textures},
                 {"font", &fonts},
                 {"sound", &sounds},
                 {"music", &musics},
                 {"style", &styles}};
    for (const auto& root : roots) {
        for (const auto& [type, mapPtr] : types) {
            std::string dir = root + "/" + type;
            loadFromDirectory(dir, *mapPtr);
        }
    }
}
// Retrieve a style JSON object by ID
const nlohmann::json& JSONLoader::getStyle(const std::string& id) const {
    auto it = styles.find(id);
    if (it == styles.end())
        throw std::out_of_range("Style ID not found: " + id);
    return it->second;
}

// Retrieve all loaded style objects
const std::unordered_map<std::string, nlohmann::json>&
JSONLoader::getAllStyles() const {
    return styles;
}

// * Individual getters
const nlohmann::json& JSONLoader::getTexture(const std::string& id) const {
    auto it = textures.find(id);
    if (it == textures.end())
        throw std::out_of_range("Texture ID not found: " + id);
    return it->second;
}

const nlohmann::json& JSONLoader::getFont(const std::string& id) const {
    auto it = fonts.find(id);
    if (it == fonts.end()) throw std::out_of_range("Font ID not found: " + id);
    return it->second;
}

const nlohmann::json& JSONLoader::getSound(const std::string& id) const {
    auto it = sounds.find(id);
    if (it == sounds.end())
        throw std::out_of_range("Sound ID not found: " + id);
    return it->second;
}

const nlohmann::json& JSONLoader::getMusic(const std::string& id) const {
    auto it = sounds.find(id);
    if (it == sounds.end())
        throw std::out_of_range("Sound ID not found: " + id);
    return it->second;
}
const nlohmann::json& JSONLoader::getLevel(const std::string& id) const {
    auto it = levels.find(id);
    if (it == levels.end())
        throw std::out_of_range("Level ID not found: " + id);
    return it->second;
}

const nlohmann::json& JSONLoader::getProjectile(const std::string& id) const {
    auto it = projectiles.find(id);
    if (it == projectiles.end())
        throw std::out_of_range("Projectile ID not found: " + id);
    return it->second;
}

const nlohmann::json& JSONLoader::getTower(const std::string& id) const {
    auto it = towers.find(id);
    if (it == towers.end())
        throw std::out_of_range("Tower ID not found: " + id);
    return it->second;
}
const nlohmann::json& JSONLoader::getEnemy(const std::string& id) const {
    auto it = enemies.find(id);
    if (it == enemies.end())
        throw std::out_of_range("Enemy ID not found: " + id);
    return it->second;
}

// * Map getter
const std::unordered_map<std::string, nlohmann::json>&
JSONLoader::getAllTextures() const {
    return textures;
}

const std::unordered_map<std::string, nlohmann::json>& JSONLoader::getAllFonts()
    const {
    return fonts;
}

const std::unordered_map<std::string, nlohmann::json>&
JSONLoader::getAllSounds() const {
    return sounds;
}

const std::unordered_map<std::string, nlohmann::json>&
JSONLoader::getAllEnemies() const {
    return enemies;
}

const std::unordered_map<std::string, nlohmann::json>&
JSONLoader::getAllLevels() const {
    return levels;
}

const std::unordered_map<std::string, nlohmann::json>&
JSONLoader::getAllProjectiles() const {
    return projectiles;
}

const std::unordered_map<std::string, nlohmann::json>&
JSONLoader::getAllTowers() const {
    return towers;
}

const std::unordered_map<std::string, nlohmann::json>&
JSONLoader::getAllMusics() const {
    return musics;
}