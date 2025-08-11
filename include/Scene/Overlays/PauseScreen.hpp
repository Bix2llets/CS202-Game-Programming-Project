#include "Overlay.hpp"

#include "Core/MouseObserver.hpp"
class PauseScreen : public Overlay, public MouseObserver {
   private:
    void constructButtons();

    std::unique_ptr<RectangularButton> resumeButton, quitButton, restartButton;
   public:
    PauseScreen(Mediator& mediator);
    ~PauseScreen();

    /**
     * @brief Updates the pause screen logic.
     */
    void update() override;

    /**
     * @brief Renders the pause screen.
     */
    void render() override;

    bool onMouseEvent(Mouse mouse, UserEvent event, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override;
    bool onScrollEvent(float delta, const sf::Vector2f &worldPosition, const sf::Vector2f &windowPosition) override {
        return false; // No scroll event handling in pause screen
    }
};