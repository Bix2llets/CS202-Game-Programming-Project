#include "Gameplay/TowerMenu.hpp"

#include <any>

#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/ButtonBuilder.hpp"
#include "GUIComponents/cursor.hpp"
#include "Gameplay/Currency.hpp"
#include "Utility/Logger.hpp"
TowerMenu::TowerMenu(const Currency& currencyRef, Mediator& superMediator)
    : basePanel{*ResourceManager::getInstance().getTexture(
          "tower_selection_base")},
      superMediator{&superMediator},
      budgetRef{currencyRef},
      petroleumDisplay{*ResourceManager::getInstance().getFont("pixel")},
      scrapDisplay{*ResourceManager::getInstance().getFont("pixel")},
      scrapIcon{currencyRef.getScraps().icon},
      petrolIcon{currencyRef.getPetroleum().icon} {
    setResourceDisplay();
    setTowerButtonDisplay();
    registerMessages();
    isTowerSelected = false;
}

void TowerMenu::update() {
    scrapDisplay.setString(std::to_string(budgetRef.getScraps().value));
    petroleumDisplay.setString(std::to_string(budgetRef.getPetroleum().value));

    for (auto& button : towerButtons) button->update();
}
void TowerMenu::render(sf::RenderStates state) const {
    sf::RenderWindow& window = Window::getInstance().getRenderWindow();
    window.draw(basePanel, state);

    window.draw(scrapDisplay, state);
    window.draw(petroleumDisplay, state);
    window.draw(scrapIcon, state);
    window.draw(petrolIcon, state);

    for (auto& button : towerButtons) button->draw(window, state);

    // auto getCircPos = [](sf::Transformable obj) {
    //     sf::CircleShape circ;
    //     circ.setRadius(3);
    //     circ.setOrigin({3, 3});
    //     circ.setPosition(obj.getPosition());
    //     circ.setFillColor(sf::Color::White);
    //     return circ;
    // };
    // window.draw(getCircPos(scrapDisplay), state);
    // window.draw(getCircPos(petroleumDisplay), state);
    // window.draw(getCircPos(scrapIcon), state);
    // window.draw(getCircPos(petrolIcon), state);
}

void TowerMenu::setResourceDisplay() {
    petroleumDisplay.setString("0");
    scrapDisplay.setString("0");
    auto fixOrigin = [](sf::Text& text) {
        text.setOrigin({0.f, (text.getLocalBounds().position.y +
                              text.getLocalBounds().size.y / 2)});
    };

    auto fixOriginIcon = [](sf::Sprite& sprite) {
        sprite.setOrigin(
            (sprite.getLocalBounds().position + sprite.getLocalBounds().size) /
            2.f);
    };
    fixOrigin(petroleumDisplay);
    fixOrigin(scrapDisplay);

    fixOriginIcon(scrapIcon);
    fixOriginIcon(petrolIcon);

    scrapIcon.setPosition(position + sf::Vector2f{40, 40});
    scrapDisplay.setPosition(position + sf::Vector2f{55, 40});
    petrolIcon.setPosition(position + sf::Vector2f{40, 80});
    petroleumDisplay.setPosition(position + sf::Vector2f{55, 80});

    petroleumDisplay.setFillColor(sf::Color::Black);
    scrapDisplay.setFillColor(sf::Color::Black);
    basePanel.setPosition(position);
    basePanel.setScale({size.x / basePanel.getLocalBounds().size.x,
                        size.y / basePanel.getLocalBounds().size.y});
}

void TowerMenu::setTowerButtonDisplay() {
    auto towerList = JSONLoader::getInstance().getAllTowers();

    const sf::Vector2f buttonSize = {80, 90};
    // * Index is the size of the vector pre-push_back

    auto getStaticTowerTexture = [](nlohmann::json jsonFile) {
        sf::RenderTexture basePlate;
        basePlate.resize(static_cast<sf::Vector2u>(sf::Vector2f{
            jsonFile["texture"]["width"], jsonFile["texture"]["height"]}));
        basePlate.clear(sf::Color::Transparent);
        sf::Sprite baseTexture(*ResourceManager::getInstance().getTexture(
            jsonFile["texture"]["base"]));
        sf::Sprite turretTexture(*ResourceManager::getInstance().getTexture(
            jsonFile["texture"]["turret"]));

        baseTexture.setOrigin(baseTexture.getLocalBounds().position +
                              baseTexture.getLocalBounds().size / 2.f);
        turretTexture.setOrigin(turretTexture.getLocalBounds().position +
                                turretTexture.getLocalBounds().size / 2.f);

        baseTexture.setPosition(static_cast<sf::Vector2f>(basePlate.getSize()) /
                                2.f);
        turretTexture.setPosition(
            static_cast<sf::Vector2f>(basePlate.getSize()) / 2.f);
        basePlate.draw(baseTexture);
        basePlate.draw(turretTexture);

        basePlate.display();
        return std::move(basePlate.getTexture());
    };
    ButtonBuilder builder(*this);
    for (std::pair<std::string, nlohmann::json> entry : towerList) {
        // * Cosmetic towers
        nlohmann::json& jsonFile = entry.second;
        sf::RenderTexture buttonRenderTexture;
        if (buttonRenderTexture.resize(static_cast<sf::Vector2u>(buttonSize))) {
            Logger::debug("Resized succ");
        } else {
            Logger::error("Resized unsucc");
        }
        buttonRenderTexture.clear(sf::Color::Transparent);
        std::unique_ptr<sf::Texture> staticTower = std::make_unique<sf::Texture>(getStaticTowerTexture(jsonFile));
        sf::Sprite towerSprite(*(staticTower.get()));
        towerTextures.push_back(std::move(staticTower));
        towerSprite.setPosition(sf::Vector2f{20.f, 20.f});
        Logger::debug(std::format("{} {}", towerSprite.getLocalBounds().size.x,
                                  towerSprite.getLocalBounds().size.y));
        // towerSprite.setScale({32.f / towerSprite.getLocalBounds().size.x,
        //                       32.f / towerSprite.getLocalBounds().size.y});
        // towerSprite.setScale({1.f, 1.f});

        towerSprite.setOrigin(towerSprite.getLocalBounds().position +
                              towerSprite.getLocalBounds().size / 2.f);

        nlohmann::json cost = jsonFile["cost"];
        // int scrapCost = cost["scrap"];
        // int petroleumCost = cost["petroleum"];
        int scrapCost = 0;
        int petroleumCost = 0;

        sf::Text petroleumCostDisplay(
            *ResourceManager::getInstance().getFont("pixel"));
        sf::Text scrapCostDisplay(
            *ResourceManager::getInstance().getFont("pixel"));

        petroleumCostDisplay.setString(std::to_string(petroleumCost));
        scrapCostDisplay.setString(std::to_string(scrapCost));
        petroleumCostDisplay.setCharacterSize(20);
        scrapCostDisplay.setCharacterSize(20);
        petroleumCostDisplay.setFillColor(sf::Color::Black);
        scrapCostDisplay.setFillColor(sf::Color::Black);

        sf::Sprite scrapIcon(
            *ResourceManager::getInstance().getTexture("scrap"));
        sf::Sprite petroleumIcon(
            *ResourceManager::getInstance().getTexture("petroleum"));

        scrapIcon.setScale({0.75f, 0.75f});
        petroleumIcon.setScale({0.75f, 0.75f});
        scrapIcon.setOrigin(scrapIcon.getLocalBounds().position +
                            scrapIcon.getLocalBounds().size / 2.f);
        petroleumIcon.setOrigin(scrapIcon.getLocalBounds().position +
                                scrapIcon.getLocalBounds().size / 2.f);

        scrapCostDisplay.setOrigin(
            {0.f, scrapCostDisplay.getLocalBounds().position.y +
                      scrapCostDisplay.getLocalBounds().size.y / 2.f});
        petroleumCostDisplay.setOrigin(
            {0.f, petroleumCostDisplay.getLocalBounds().position.y +
                      petroleumCostDisplay.getLocalBounds().size.y / 2.f});
        petroleumCostDisplay.setPosition({25, 50});
        petroleumIcon.setPosition({10, 50});
        scrapCostDisplay.setPosition({25, 75});
        scrapIcon.setPosition({10, 75});

        buttonRenderTexture.draw(towerSprite);
        buttonRenderTexture.draw(petroleumCostDisplay);
        buttonRenderTexture.draw(scrapCostDisplay);
        buttonRenderTexture.draw(scrapIcon);
        buttonRenderTexture.draw(petroleumIcon);

        buttonRenderTexture.display();

        sf::Texture buttonTexture = std::move(buttonRenderTexture.getTexture());

        combinedTowerTextures.push_back(
            std::make_unique<sf::Texture>(buttonRenderTexture.getTexture()));
        int row = towerButtons.size() / BUTTON_PER_ROW;
        int col = towerButtons.size() % BUTTON_PER_ROW;

        sf::Vector2f buttonPosition;
        buttonPosition.x = col * (buttonGap.x + buttonSize.x) + position.x + 10;
        buttonPosition.y =
            row * (buttonGap.y + buttonSize.y) + position.y + 120;

        std::unique_ptr<Button> button =
            builder.reset()
                .loadJson("background_basic")
                .setBackground(combinedTowerTextures.back().get())
                .setPosition({buttonPosition})
                .setSize(buttonSize)
                .setCallback([this, towerSprite](Button* button) {
                    Logger::debug("Button presseed");
                    notify("tower_button_pressed", 0, towerSprite);
                })
                .build();
        renderTexes.push_back(std::move(buttonRenderTexture));
        towerButtons.push_back(std::move(button));
    }
}

void TowerMenu::onLoad() {
    for (auto& button : towerButtons) {
        button->subscribeMouseAll(InputManager::getInstance().getMouseState());
    }
}

void TowerMenu::onUnload() {
    for (auto& button : towerButtons) {
        button->unSubscribeMouseAll(
            InputManager::getInstance().getMouseState());
    }
}

void TowerMenu::registerMessages() {
    subscribe("tower_button_pressed", [this](std::any sender, std::any data) {
        sf::Sprite buttonBackground = std::any_cast<sf::Sprite>(data);
        Cursor::getInstance().setRenderImage(buttonBackground);
        isTowerSelected = true;
    });
    subscribe("press_inside", [this](std::any, std::any) {
        if (isTowerSelected == false) return;
        Cursor::getInstance().removeRenderImage();
        isTowerSelected = false;
    });
    subscribe("press_outside", [this](std::any sender, std::any data) {
        if (isTowerSelected == false) return;
        sf::Vector2f worldPosition = std::any_cast<sf::Vector2f>(data);
        Cursor::getInstance().removeRenderImage();
    });
}