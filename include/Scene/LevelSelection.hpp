#include "Scene/Scene.hpp"


class LevelSelection : public Scene {

    private:

    public:
    LevelSelection();
    ~LevelSelection();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    void onLoad() override;
    void onUnload() override;
};