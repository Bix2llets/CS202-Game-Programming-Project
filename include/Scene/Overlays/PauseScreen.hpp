#include "Overlay.hpp"

class PauseScreen : public Overlay {
   private:

   public:
    PauseScreen();
    ~PauseScreen() = default;

    /**
     * @brief Updates the pause screen logic.
     */
    void update() override;

    /**
     * @brief Renders the pause screen.
     */
    void render() override;
};