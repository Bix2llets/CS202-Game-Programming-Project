#include "Scene/Scene.hpp"

#include "GUIComponents/TextBox.hpp"
#include "Core/JSONLoader.hpp"
class TestScene : public Scene{
   public:
    TestScene() {
        textBox.setSize({200.f, 50.f})
            .setPosition({100.f, 100.f})
            .setText("Test Input")
            .setStyle(JSONLoader::getInstance().getStyle("basic_button"))
            .adjust();
        Logger::success("Constructed TestScene");
        textBox.subscribeMouse(Mouse::Left, UserEvent::Press, InputManager::getInstance().getMouseState());
    };
    TextBox textBox;
    void draw(sf::RenderTarget &target, sf::RenderStates state) const override {
        // No-op for test scene
        target.draw(textBox, state);
    }

    void update() override {
        textBox.update();
    }

    void onLoad() override {
        // No components to register in a test scene
    }

    void onUnload() override {
        // No components to unregister in a test scene
    }
};