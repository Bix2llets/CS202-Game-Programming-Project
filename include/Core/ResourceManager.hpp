/**
 * @file ResourceManager.hpp
 * @brief Declares the ResourceManager class for managing game resources.
 */
#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <map>
#include <memory>
#include <string>
/**
 * @class ResourceManager
 * @brief Manages loading, storing, and accessing game resources such as textures, sounds, and fonts.
 */
class ResourceManager {
   private:
    std::map<std::string, std::unique_ptr<sf::Texture>> textures; ///< Loaded textures.
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers; ///< Loaded sound buffers.
    std::map<std::string, std::unique_ptr<sf::Font>> fonts; ///< Loaded fonts.
    std::list<std::unique_ptr<sf::Sound>> playingSounds; ///< Currently playing sounds.

    ResourceManager(const ResourceManager &rhs) = delete;
    ResourceManager operator=(const ResourceManager &rhs) = delete;

    /**
     * @brief Removes sounds from the playingSounds list that have finished playing.
     */
    void freeSound();

   public:
    /**
     * @brief Default constructor.
     */
    ResourceManager() = default;

    /**
     * @brief Move constructor.
     */
    ResourceManager(ResourceManager &&rhs) noexcept = default;

    /**
     * @brief Loads a sound buffer from file and stores it with the given ID.
     * @param path Path to the sound file.
     * @param ID Key to identify the loaded sound buffer.
     */
    void loadSound(const std::string &path, const std::string &ID);

    /**
     * @brief Loads a texture from file and stores it with the given ID.
     * @param path Path to the texture file.
     * @param ID Key to identify the loaded texture.
     */
    void loadTexture(const std::string &path, const std::string &ID);

    /**
     * @brief Loads a font from file and stores it with the given ID.
     * @param path Path to the font file.
     * @param ID Key to identify the loaded font.
     */
    void loadFont(const std::string &path, const std::string &ID);

    /**
     * @brief Destructor. Frees all loaded fonts, sound buffers, and textures.
     */
    ~ResourceManager();

    /**
     * @brief Plays a sound by ID. The sound buffer must be loaded first.
     * @param ID Key of the sound buffer to play.
     */
    void playSound(const std::string &ID);

    /**
     * @brief Retrieves a pointer to a loaded texture by ID.
     * @param ID Key of the texture to retrieve.
     * @return Pointer to the texture, or nullptr if not found.
     */
    const sf::Texture *const getTexture(const std::string &ID) const;

    /**
     * @brief Retrieves a pointer to a loaded font by ID.
     * @param ID Key of the font to retrieve.
     * @return Pointer to the font, or nullptr if not found.
     */
    const sf::Font *const getFont(const std::string &ID) const;
};