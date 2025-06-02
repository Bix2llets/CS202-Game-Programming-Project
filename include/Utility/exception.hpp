#include <exception>
#include <string>
class GameException : public std::exception {
    private:
    std::string msg;
    public: 
    GameException(const char* message) : msg{message} {}
    GameException(const std::string &message) : msg{message} {} 
    ~GameException() = default;
    virtual const char* what() const noexcept {return msg.c_str(); }
};