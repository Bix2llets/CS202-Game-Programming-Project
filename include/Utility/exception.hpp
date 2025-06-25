/**
 * @file exception.hpp
 * @brief Declares the GameException class for custom game exceptions.
 */
#include <exception>
#include <string>
/**
 * @class GameException
 * @brief Custom exception class for game-specific errors.
 */
class GameException : public std::exception {
    private:
    std::string msg; ///< Error message.
    public: 
    /**
     * @brief Constructs a GameException with a C-string message.
     * @param message The error message.
     */
    GameException(const char* message) : msg{message} {}
    /**
     * @brief Constructs a GameException with a std::string message.
     * @param message The error message.
     */
    GameException(const std::string &message) : msg{message} {} 
    /**
     * @brief Destructor.
     */
    ~GameException() = default;
    /**
     * @brief Returns the error message.
     * @return C-string error message.
     */
    virtual const char* what() const noexcept {return msg.c_str(); }
};