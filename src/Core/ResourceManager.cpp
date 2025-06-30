#include "Core/ResourceManager.hpp"
#include "Utility/logger.hpp"
void ResourceManager::freeSound() {
	for (auto it = playingSounds.begin(); it != playingSounds.end(); ) {
        if (it->get()->getStatus() == sf::Sound::Status::Stopped) {
            auto place = it;
            it++;
            playingSounds.erase(place);
        } 
        else
            it++;
    }
}

void ResourceManager::loadSound(const std::string &path, const std::string &ID) {
    if (textures.find(ID) != textures.end()) {
        Logger::error("Sound ID collision while importing: " + ID);
        return;
    }
    soundBuffers[ID] = std::make_unique<sf::SoundBuffer>(path);
}

void ResourceManager::loadFont(const std::string &path, const std::string &ID) {
    if (fonts.find(ID) != fonts.end()) {
        Logger::error("Font ID collision while importing: " + ID);
        return;
    }

    fonts.at(ID) = std::make_unique<sf::Font>(path);
}

void ResourceManager::loadTexture(const std::string &path, const std::string &ID) {
    if (textures.find(ID) != textures.end()) {
        Logger::error("Texture ID collision while importing: " + ID);
        return;
    }
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
    std::unique_ptr<sf::Sound> sound = std::make_unique<sf::Sound>(*(soundBuffers[ID].get()));
    sound->play();
    playingSounds.push_back(std::move(sound));
    // std::cerr << playingSounds.size();
}

const sf::Texture *const ResourceManager::getTexture(const std::string &ID) const {
    if (textures.find(ID) == textures.end()) {
        Logger::error("Texture ID not found: " + ID);
        return nullptr;
    }
    return textures.at(ID).get();

}
const sf::Font *const ResourceManager::getFont(const std::string &ID) const {
    if (fonts.find(ID) == fonts.end()) {
        Logger::error("Texture ID not found: " + ID);
        return nullptr;
    }
    return fonts.at(ID).get();


}

void ResourceManager::setSoundVolume(int volume) {
    soundVolume = volume;
}

void ResourceManager::setMusicVolume(int volume) {
    musicVolume = volume;
}

int ResourceManager::getSoundVolume() const {
    return soundVolume;
}

int ResourceManager::getMusicVolume() const {
    return musicVolume;
}