#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <list>
#include <map>
#include <memory>
#include <string>
/**
 * @class ResourceManager
 * @brief Centralized manager for loading, storing, and accessing game resources
 * such as textures, sounds, fonts, and music.
 */

class ResourceManager {
   private:
    std::map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
    std::map<std::string, std::unique_ptr<sf::Font>> fonts;
    std::map<std::string, std::unique_ptr<sf::Music>> musics;

    /**
     * @brief List of currently playing sound instances.
     *
     * Used to manage lifetime and cleanup of sound effects.
     */
    std::list<std::unique_ptr<sf::Sound>> playingSounds;
    int soundVolume;
    int musicVolume;

    /**
     * @brief Removes finished sounds from the playingSounds list.
     *
     * Called internally after playing a sound to clean up stopped sound
     * instances.
     */
    void freeSound();
    ResourceManager(const ResourceManager &rhs) = delete;
    ResourceManager operator=(const ResourceManager &rhs) = delete;
    ResourceManager() = default;
   public:
    /**
     * @brief Loads a sound buffer from file and stores it with the given ID.
     * @param file JSON object containing sound metadata (must include path and
     * ID fields).
     * @throws std::runtime_error if the file cannot be loaded or ID is
     * missing/duplicate.
     * @note Call this before attempting to play a sound with playSound().
     */
    void loadSound(const nlohmann::json &file);

    /**
     * @brief Loads a sound buffer from a file path and stores it with the given
     * ID.
     * @param path Path to the sound file.
     * @param ID Unique key to identify the loaded sound buffer.
     * @throws std::runtime_error if the file cannot be loaded or ID is
     * missing/duplicate.
     * @note Use this for programmatic asset loading outside of JSON.
     * @deprecated This method will be removed in the near future. Use
     * JSON-based loading instead.
     */
    [[deprecated(
        "Will be removed in near future, prefer centralized loading on "
        "initialization")]]
    void loadSound(const std::string &path, const std::string &ID);

    /**
     * @brief Loads a texture from file and stores it with the given ID.
     * @param file JSON object containing texture metadata (must include path
     * and ID fields).
     * @throws std::runtime_error if the file cannot be loaded or ID is
     * missing/duplicate.
     * @note Call this before attempting to retrieve a texture with
     * getTexture().
     */
    void loadTexture(const nlohmann::json &file);

    /**
     * @brief Loads a texture from a file path and stores it with the given ID.
     * @param path Path to the texture file.
     * @param ID Unique key to identify the loaded texture.
     * @throws std::runtime_error if the file cannot be loaded or ID is
     * missing/duplicate.
     * @note Use this for programmatic asset loading outside of JSON.
     * @deprecated This method will be removed in the near future. Use
     * JSON-based loading instead.
     */
    [[deprecated(
        "Will be removed in near future, prefer centralized loading on "
        "initialization")]]
    void loadTexture(const std::string &path, const std::string &ID);

    /**
     * @brief Loads a font from file and stores it with the given ID.
     * @param file JSON object containing font metadata (must include path and
     * ID fields).
     * @throws std::runtime_error if the file cannot be loaded or ID is
     * missing/duplicate.
     * @note Call this before attempting to retrieve a font with getFont().
     */
    void loadFont(const nlohmann::json &file);

    /**
     * @brief Loads a font from a file path and stores it with the given ID.
     * @param path Path to the font file.
     * @param ID Unique key to identify the loaded font.
     * @throws std::runtime_error if the file cannot be loaded or ID is
     * missing/duplicate.
     * @note Use this for programmatic asset loading outside of JSON.
     * @deprecated This method will be removed in the near future. Use
     * JSON-based loading instead.
     */
    [[deprecated(
        "Will be removed in near future, prefer centralized loading on "
        "initialization")]]
    void loadFont(const std::string &path, const std::string &ID);

    /**
     * @brief Loads a music track from file and stores it with the given ID.
     * @param file JSON object containing music metadata (must include path and
     * ID fields).
     * @throws std::runtime_error if the file cannot be loaded or ID is
     * missing/duplicate.
     * @note Call this before attempting to play music with playMusic().
     */
    void loadMusic(const nlohmann::json &file);

    /**
     * @brief Loads a music track from a file path and stores it with the given
     * ID.
     * @param path Path to the music file.
     * @param ID Unique key to identify the loaded music track.
     * @throws std::runtime_error if the file cannot be loaded or ID is
     * missing/duplicate.
     * @note Use this for programmatic asset loading outside of JSON.
     * @deprecated This method will be removed in the near future. Use
     * JSON-based loading instead.
     */
    [[deprecated(
        "Will be removed in near future, prefer centralized loading on "
        "initialization")]]
    void loadMusic(const std::string &path, const std::string &ID);
    /**
     * @brief Destructor. Frees all loaded fonts, sound buffers, textures, and
     * music tracks.
     * @details All resources and currently playing sounds are properly cleaned
     * up.
     */
    ~ResourceManager();

    /**
     * @brief Plays a sound effect by ID.
     * @param ID Key of the sound buffer to play (must be loaded first).
     * @throws std::runtime_error if the sound buffer is not found.
     * @note Multiple sounds can play simultaneously. Volume is controlled by
     * setSoundVolume().
     * @warning Playing too many sounds at once may exhaust system resources.
     */
    void playSound(const std::string &ID);

    /**
     * @brief Retrieves a pointer to a loaded texture by ID.
     * @param ID Key of the texture to retrieve.
     * @return Pointer to the texture, or nullptr if not found.
     * @note Returned pointer is owned by ResourceManager; do not delete.
     */
    const sf::Texture *const getTexture(const std::string &ID) const;

    /**
     * @brief Retrieves a pointer to a loaded font by ID.
     * @param ID Key of the font to retrieve.
     * @return Pointer to the font, or nullptr if not found.
     * @note Returned pointer is owned by ResourceManager; do not delete.
     */
    const sf::Font *const getFont(const std::string &ID) const;

    /**
     * @brief Sets the sound effects volume for all currently and future playing
     * sounds.
     * @param volume Volume level (0-100).
     * @note Values outside [0,100] will be clamped. Affects all active and new
     * sound effects.
     */
    void setSoundVolume(int volume);

    /**
     * @brief Gets the current sound effects volume.
     * @return Current sound volume (0-100).
     */
    int getSoundVolume() const;

    /**
     * @brief Sets the music volume for all currently and future playing music
     * tracks.
     * @param volume Volume level (0-100).
     * @note Values outside [0,100] will be clamped. Affects all active and new
     * music tracks.
     */
    void setMusicVolume(int volume);

    /**
     * @brief Gets the current music volume.
     * @return Current music volume (0-100).
     */
    int getMusicVolume() const;

    /**
     * @brief Plays a music track by ID.
     * @param id Key of the music track to play (must be loaded first).
     * @throws std::runtime_error if the music track is not found.
     * @warning Playing multiple music tracks simultaneously may cause audio
     * overlap.
     */
    void playMusic(std::string id);

    /**
     * @brief Stops playback of all music tracks.
     * @note Useful for pausing or switching scenes. Does not affect sound
     * effects.
     */
    void stopAllMusic();

    /**
     * @brief Stops playback of a specific music track by ID.
     * @param id Key of the music track to stop.
     * @note If the track is not playing, this has no effect.
     */
    void stopMusic(std::string id);


    static ResourceManager& getInstance() {
        static ResourceManager result;
        return result;
    }
   private:
    void validateJson(const nlohmann::json &file);
};