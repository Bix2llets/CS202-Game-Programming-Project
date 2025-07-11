#include "Core/ResourceManager.hpp"

#include "Utility/logger.hpp"
void ResourceManager::freeSound() {
    for (auto it = playingSounds.begin(); it != playingSounds.end();) {
        if (it->get()->getStatus() == sf::Sound::Status::Stopped) {
            auto place = it;
            it++;
            playingSounds.erase(place);
        } else
            it++;
    }
}

void ResourceManager::loadSound(const nlohmann::json &file) {
    validateJson(file);
    std::string ID = file["id"];
    std::string path = file["path"];
    if (soundBuffers.find(ID) != soundBuffers.end()) {
        Logger::error("Sound ID collision while importing: " + ID);
        return;
    }
    auto buffer = std::make_unique<sf::SoundBuffer>();
    if (!buffer->loadFromFile(path)) {
        Logger::error("Failed to load sound from path: " + path);
        return;
    }
    soundBuffers[ID] = std::move(buffer);
    Logger::success("Successfully loaded sound: " + ID + " from " + path);
}

void ResourceManager::loadSound(const std::string &path,
                                const std::string &ID) {
    if (soundBuffers.find(ID) != soundBuffers.end()) {
        Logger::error("Sound ID collision while importing: " + ID);
        return;
    }
    auto buffer = std::make_unique<sf::SoundBuffer>();
    if (!buffer->loadFromFile(path)) {
        Logger::error("Failed to load sound from path: " + path);
        return;
    }
    soundBuffers[ID] = std::move(buffer);
    Logger::success("Successfully loaded sound: " + ID + " from " + path);
}

void ResourceManager::loadFont(const nlohmann::json &file) {
    validateJson(file);
    std::string ID = file["id"];
    std::string path = file["path"];
    if (fonts.find(ID) != fonts.end()) {
        Logger::error("Font ID collision while importing: " + ID);
        return;
    }
    auto font = std::make_unique<sf::Font>();
    if (!font->openFromFile(path)) {
        Logger::error("Failed to load font from path: " + path);
        return;
    }
    fonts[ID] = std::move(font);
    Logger::success("Successfully loaded font: " + ID + " from " + path);
}

void ResourceManager::loadFont(const std::string &path, const std::string &ID) {
    if (fonts.find(ID) != fonts.end()) {
        Logger::error("Font ID collision while importing: " + ID);
        return;
    }
    auto font = std::make_unique<sf::Font>();
    if (!font->openFromFile(path)) {
        Logger::error("Failed to load font from path: " + path);
        return;
    }
    fonts[ID] = std::move(font);
    Logger::success("Successfully loaded font: " + ID + " from " + path);
}

void ResourceManager::loadTexture(const nlohmann::json &file) {
    validateJson(file);
    std::string ID = file["id"];
    std::string path = file["path"];
    if (textures.find(ID) != textures.end()) {
        Logger::error("Texture ID collision while importing: " + ID);
        return;
    }
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        Logger::error("Failed to load texture from path: " + path);
        return;
    }
    textures[ID] = std::move(texture);
    Logger::success("Successfully loaded texture: " + ID + " from " + path);
}

void ResourceManager::loadTexture(const std::string &path,
                                  const std::string &ID) {
    if (textures.find(ID) != textures.end()) {
        Logger::error("Texture ID collision while importing: " + ID);
        return;
    }
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        Logger::error("Failed to load texture from path: " + path);
        return;
    }
    textures[ID] = std::move(texture);
    Logger::success("Successfully loaded texture: " + ID + " from " + path);
}

void ResourceManager::loadMusic(const nlohmann::json &file) {
    validateJson(file);
    std::string ID = file["id"];
    std::string path = file["path"];
    if (musics.find(ID) != musics.end()) {
        Logger::error("Music ID collision while importing: " + ID);
        return;
    }
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(path)) {
        Logger::error("Failed to load music from path: " + path);
        return;
    }
    musics[ID] = std::move(music);
    Logger::success("Successfully loaded music: " + ID + " from " + path);
}

void ResourceManager::loadMusic(const std::string &path,
                                const std::string &ID) {
    if (musics.find(ID) != musics.end()) {
        Logger::error("Music ID collision while importing: " + ID);
        return;
    }
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(path)) {
        Logger::error("Failed to load music from path: " + path);
        return;
    }
    musics[ID] = std::move(music);
    Logger::success("Successfully loaded music: " + ID + " from " + path);
}
ResourceManager::~ResourceManager() {
    textures.clear();
    soundBuffers.clear();
    fonts.clear();
}

void ResourceManager::playSound(const std::string &ID) {
    freeSound();
    if (soundBuffers.find(ID) == soundBuffers.end()) {
        Logger::error("Sound ID not found: " + ID);
        return;
    }
    std::unique_ptr<sf::Sound> sound =
        std::make_unique<sf::Sound>(*(soundBuffers[ID].get()));
    sound->play();
    playingSounds.push_back(std::move(sound));
    // std::cerr << playingSounds.size();
}

const sf::Texture *const ResourceManager::getTexture(
    const std::string &ID) const {
    if (textures.find(ID) == textures.end()) {
        Logger::error("Font ID not found: " + ID);
        return nullptr;
    }
    return textures.at(ID).get();
}
const sf::Font *const ResourceManager::getFont(const std::string &ID) const {
    if (fonts.find(ID) == fonts.end()) {
        Logger::error("Font ID not found: " + ID);
        return nullptr;
    }
    return fonts.at(ID).get();
}

void ResourceManager::setSoundVolume(int volume) { soundVolume = volume; }

void ResourceManager::setMusicVolume(int volume) { musicVolume = volume; }

int ResourceManager::getSoundVolume() const { return soundVolume; }

int ResourceManager::getMusicVolume() const { return musicVolume; }

void ResourceManager::validateJson(const nlohmann::json &jsonFile) {
    std::cerr << jsonFile;
    if (!jsonFile.contains("id") || !jsonFile.contains("path"))
        throw std::invalid_argument(
            "JSON file must contain 'ID' and 'path' fields.");
}