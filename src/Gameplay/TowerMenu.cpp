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
#include "Utility/WindowScale.hpp"
TowerMenu::TowerMenu(const Currency& currencyRef, Mediator& superMediator)
    : basePanel{*ResourceManager::getInstance().getTexture(
          "tower_selection_base")},
      superMediator{&superMediator},
      budgetRef{currencyRef},
      petroleumDisplay{*ResourceManager::getInstance().getFont("pixel")},
      scrapDisplay{*ResourceManager::getInstance().getFont("pixel")},
      scrapIcon{currencyRef.getScraps().icon},
      petrolIcon{currencyRef.getPetroleum().icon} {
    isTowerSelected = false;

    position.y = 0;
    position.x = GameConstants::DEFAULT_WINDOW_WIDTH - buttonGap.x -
                 borderSize.x * 2.f - buttonSize.x * BUTTON_PER_ROW;
    baseRectangle.setOrigin({0.f, 0.f});
    baseRectangle.setPosition(position);
    baseRectangle.setSize({GameConstants::DEFAULT_WINDOW_WIDTH - position.x,
                           GameConstants::DEFAULT_WINDOW_HEIGHT});
    baseRectangle.setFillColor(sf::Color(93, 153, 189, 255));
    setResourceDisplay();
    setTowerButtonDisplay();
    registerMessages();
}

void TowerMenu::update() {
    scrapDisplay.setString(std::to_string(budgetRef.getScraps().value));
    petroleumDisplay.setString(std::to_string(budgetRef.getPetroleum().value));

    for (auto& button : towerButtons) button->update();
}
void TowerMenu::render(sf::RenderStates state) const {
    sf::RenderWindow& window = Window::getInstance().getRenderWindow();
    window.draw(baseRectangle, state);

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

    scrapIcon.setPosition(position + sf::Vector2f{borderSize.x + scrapIcon.getLocalBounds().size.x / 2, 40});
    scrapDisplay.setPosition(scrapIcon.getPosition() + sf::Vector2f{scrapIcon.getLocalBounds().size.x / 2.f, 0.f} + sf::Vector2f{10, 0});
    petrolIcon.setPosition(position + sf::Vector2f{borderSize.x + petrolIcon.getLocalBounds().size.x / 2, 80});
    petroleumDisplay.setPosition(petrolIcon.getPosition() + sf::Vector2f{petrolIcon.getLocalBounds().size.x / 2.f, 0.f} + sf::Vector2f{10, 0});

    petroleumDisplay.setFillColor(sf::Color::White);
    scrapDisplay.setFillColor(sf::Color::White);
}

void TowerMenu::setTowerButtonDisplay() {
    auto towerList = JSONLoader::getInstance().getAllTowers();

    const sf::Vector2f buttonSize = {80, 120};
    // * Index is the size of the vector pre-push_back

    auto getStaticTowerTexture = [](nlohmann::json jsonFile) {
        sf::RenderTexture basePlate;
        if (basePlate.resize(static_cast<sf::Vector2u>(
                sf::Vector2f{jsonFile["texture"]["width"],
                             jsonFile["texture"]["height"]}))) {
            Logger::success("Resize succ");
        } else {
            Logger::error("Resize failed");
        }
        basePlate.clear(sf::Color::Transparent);
        nlohmann::json animationInfo = jsonFile["texture"]["turret_animation"];
        sf::Sprite baseTexture(*ResourceManager::getInstance().getTexture(
            jsonFile["texture"]["base"]));
        sf::Sprite turretTexture(*ResourceManager::getInstance().getTexture(
            animationInfo["texture_id"]));

        turretTexture.setTextureRect(
            {sf::Vector2i{0, 0}, sf::Vector2i{(int)animationInfo["width"],
                                              (int)animationInfo["height"]}});

        turretTexture.setOrigin(
            {animationInfo["center_x"], animationInfo["center_y"]});

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
        buttonRenderTexture.clear(sf::Color(228, 215, 170, 255));
        std::unique_ptr<sf::Texture> staticTower =
            std::make_unique<sf::Texture>(getStaticTowerTexture(jsonFile));
        sf::Sprite towerSprite(*(staticTower.get()));
        towerTextures.push_back(std::move(staticTower));
        towerSprite.setPosition(sf::Vector2f{buttonSize.x / 2, 30.f});
        towerSprite.setScale({1.f, 1.f});
        Logger::debug(std::format("{} {}", towerSprite.getLocalBounds().size.x,
                                  towerSprite.getLocalBounds().size.y));
        // towerSprite.setScale({32.f / towerSprite.getLocalBounds().size.x,
        //                       32.f / towerSprite.getLocalBounds().size.y});
        // towerSprite.setScale({1.f, 1.f});

        towerSprite.setOrigin(towerSprite.getLocalBounds().position +
                              towerSprite.getLocalBounds().size / 2.f);

        nlohmann::json cost = jsonFile["cost"];
        int scrapCost = 0;
        int petroleumCost = 0;
        scrapCost = cost["scrap"];
        petroleumCost = cost["petroleum"];

        sf::Text petroleumCostDisplay(
            *ResourceManager::getInstance().getFont("pixel"));
        sf::Text scrapCostDisplay(
            *ResourceManager::getInstance().getFont("pixel"));

        petroleumCostDisplay.setString(std::to_string(petroleumCost));
        scrapCostDisplay.setString(std::to_string(scrapCost));
        petroleumCostDisplay.setCharacterSize(20);
        scrapCostDisplay.setCharacterSize(20);
        petroleumCostDisplay.setFillColor(sf::Color(22, 50, 60, 255));
        scrapCostDisplay.setFillColor(sf::Color(22, 50, 60, 255));

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
        int yOffset = 35;
        petroleumCostDisplay.setPosition({25, towerSprite.getPosition().y + yOffset+ 5});
        petroleumIcon.setPosition({10, towerSprite.getPosition().y + yOffset + 5});
        scrapCostDisplay.setPosition({25, towerSprite.getPosition().y + yOffset + 30});
        scrapIcon.setPosition({10, towerSprite.getPosition().y + yOffset + 30});

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
                .setCallback([this, towerSprite, entry](Button* button) {
                    Logger::debug("Button presseed");
                    notify("tower_button_pressed", entry.first, towerSprite);
                })
                .build();
        renderTexes.push_back(std::move(buttonRenderTexture));
        towerButtons.push_back(std::move(button));
    }
}

void TowerMenu::onLoad() {
    // for (auto& button : towerButtons) {
    //     button->subscribeMouseAll(InputManager::getInstance().getMouseState());
    // }
}

void TowerMenu::onUnload() {
    // for (auto& button : towerButtons) {
    //     button->unSubscribeMouseAll(
    //         InputManager::getInstance().getMouseState());
    // }
}

void TowerMenu::registerMessages() {
    subscribe("tower_button_pressed", [this](std::any id, std::any sprite) {
        sf::Sprite buttonBackground = std::any_cast<sf::Sprite>(sprite);
        std::string towerId = std::any_cast<std::string>(id);
        Cursor::getInstance().setRenderImage(buttonBackground);
        Cursor::getInstance().setCarryingTower(towerId);
        isTowerSelected = true;
    });
    subscribe("press_inside", [this](std::any, std::any) {
        if (isTowerSelected == false) return;
        Cursor::getInstance().removeRenderImage();
        Cursor::getInstance().clearCarryingTower();
        isTowerSelected = false;
    });
    subscribe("press_outside", [this](std::any sender, std::any data) {
        if (isTowerSelected == false) return;
        sf::Vector2f worldPosition = std::any_cast<sf::Vector2f>(data);
        superMediator->notify("place_tower_cursor", 0, worldPosition);

        Cursor::getInstance().removeRenderImage();
        Cursor::getInstance().clearCarryingTower();
        isTowerSelected = false;
    });
}

bool TowerMenu::onMouseEvent(Mouse mouse, UserEvent event,
                             const sf::Vector2f& worldPosition,
                             const sf::Vector2f& windowPosition) {
    bool isProcessed = false;

    if (mouse == Mouse::Left && event == UserEvent::Press) {
        for (std::unique_ptr<Button>& button : towerButtons)
            if (button->onMouseEvent(mouse, event, worldPosition,
                                     windowPosition))
                return true;

        if (isTowerSelected &&
            WindowScale::screenScale(baseRectangle.getGlobalBounds())
                .contains(windowPosition)) {
            notify("press_inside");
            return true;
        }
        if (isTowerSelected) {
            notify("press_outside", 0, worldPosition);
            return true;
        }
        return false;
    }

    if (event == UserEvent::Move) {
        for (std::unique_ptr<Button>& button : towerButtons) {
            if (button->onMouseEvent(Mouse::None, event, worldPosition,
                                     windowPosition))
                continue;
            if (button->onMouseEvent(Mouse::Left, event, worldPosition,
                                     windowPosition))
                continue;
            if (button->onMouseEvent(Mouse::Right, event, worldPosition,
                                     windowPosition))
                continue;
            if (button->onMouseEvent(Mouse::Middle, event, worldPosition,
                                     windowPosition))
                continue;
        }
        return false;
    }

    if (event == UserEvent::Release && mouse == Mouse::Left) {
        bool isInBound = false;
        for (auto& button : towerButtons) {
            isInBound |= button->onMouseEvent(mouse, event, worldPosition,
                                              windowPosition);
        }
        return isInBound;
    }
    return false;
}

bool TowerMenu::onScrollEvent(float delta, const sf::Vector2f& worldPosition,
                              const sf::Vector2f& windowPosition) {
    return false;
}