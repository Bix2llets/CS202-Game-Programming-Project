#include "Gameplay/TowerMenu.hpp"

#include <any>

#include "Core/InputManager.hpp"
#include "Core/JSONLoader.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Window.hpp"
#include "GUIComponents/RectangularButtonBuilder.hpp"
#include "GUIComponents/cursor.hpp"
#include "Gameplay/Currency.hpp"
#include "Scene/Level.hpp"
#include "Utility/Logger.hpp"
#include "Utility/Scaler.hpp"
#include "Utility/aligner.hpp"
TowerMenu::TowerMenu(const Currency& currencyRef, Mediator& superMediator)
    : basePanel{*ResourceManager::getInstance().getTexture(
          "tower_selection_base")},
      level{level},
      budgetRef{currencyRef},
      petroleumDisplay{*ResourceManager::getInstance().getFont("pixel")},
      scrapDisplay{*ResourceManager::getInstance().getFont("pixel")},
      scrapIcon{currencyRef.getScraps().icon},
      petrolIcon{currencyRef.getPetroleum().icon} {

    for (auto entry : JSONLoader::getInstance().getAllTowers()) {
        std::unique_ptr<Tower> tower = TowerFactory::createFromJson(entry.second, *level, sf::Vector2f(300, 300));
        
        if(tower->isBuildable()) {
            towerList[entry.first] = std::move(tower);
            Logger::debug(std::format("Tower {} is added to towerList, new size: {}", entry.first, towerList.size()));
        }
    }

    position.y = 0;
    position.x = GameConstants::DEFAULT_WINDOW_WIDTH - buttonGap.x -
                 borderSize.x * 2.f - buttonSize.x * BUTTON_PER_ROW;
    baseRectangle.setOrigin({0.f, 0.f});
    baseRectangle.setPosition(position);
    baseRectangle.setSize({GameConstants::DEFAULT_WINDOW_WIDTH - position.x,
                           GameConstants::DEFAULT_WINDOW_HEIGHT});
    baseRectangle.setFillColor(sf::Color(93, 153, 189, 255));

    setResourceDisplay();
    registerMessages();
    setTowerButtonDisplay();
    isTowerSelected = false;
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

    scrapIcon.setPosition(
        position +
        sf::Vector2f{borderSize.x + scrapIcon.getLocalBounds().size.x / 2, 40});
    scrapDisplay.setPosition(
        scrapIcon.getPosition() +
        sf::Vector2f{scrapIcon.getLocalBounds().size.x / 2.f, 0.f} +
        sf::Vector2f{10, 0});
    petrolIcon.setPosition(
        position +
        sf::Vector2f{borderSize.x + petrolIcon.getLocalBounds().size.x / 2,
                     80});
    petroleumDisplay.setPosition(
        petrolIcon.getPosition() +
        sf::Vector2f{petrolIcon.getLocalBounds().size.x / 2.f, 0.f} +
        sf::Vector2f{10, 0});

    petroleumDisplay.setFillColor(sf::Color::White);
    scrapDisplay.setFillColor(sf::Color::White);
}

void TowerMenu::setTowerButtonDisplay() {
    auto towerList = JSONLoader::getInstance().getAllTowers();

    const sf::Vector2f buttonSize = {80, 120};
    const std::string buttonStyleID = "background_basic";
    // * Index is the size of the vector pre-push_back

    // auto getStaticTowerTexture = [](nlohmann::json jsonFile) {
    //     sf::RenderTexture basePlate;
    //     if (basePlate.resize(static_cast<sf::Vector2u>(sf::Vector2f{
    //             jsonFile["texture"]["width"], jsonFile["texture"]["height"]})))
    //         {
    //             Logger::success("Resize succ");
    //         }
    //         else {
    //             Logger::error("Resize failed");
    //         }
    //     basePlate.clear(sf::Color::Transparent);
    //     sf::Sprite baseTexture(*ResourceManager::getInstance().getTexture(
    //         jsonFile["texture"]["base"]));
    //     sf::Sprite turretTexture(*ResourceManager::getInstance().getTexture(
    //         jsonFile["texture"]["turret"]));

    //     baseTexture.setOrigin(baseTexture.getLocalBounds().position +
    //                           baseTexture.getLocalBounds().size / 2.f);
    //     turretTexture.setOrigin(turretTexture.getLocalBounds().position +
    //                             turretTexture.getLocalBounds().size / 2.f);

    //     baseTexture.setPosition(static_cast<sf::Vector2f>(basePlate.getSize()) /
    //                             2.f);
    //     turretTexture.setPosition(
    //         static_cast<sf::Vector2f>(basePlate.getSize()) / 2.f);
    //     basePlate.draw(baseTexture);
    //     basePlate.draw(turretTexture);

        basePlate.display();
        return std::move(basePlate.getTexture());
    };
    RectangularButtonBuilder builder(*this);

    for (std::pair<std::string, nlohmann::json> entry : towerList) {
        // * Cosmetic towers
        nlohmann::json& jsonFile = entry.second;
        if (jsonFile["buildable"] == false) {
            Logger::debug(
                std::format("Skipping cosmetic tower: {}", entry.first));
            continue;
        }
        sf::RenderTexture buttonRenderTexture;
        if (buttonRenderTexture.resize(static_cast<sf::Vector2u>(buttonSize))) {
            Logger::debug("Resized successfully");
        } else {
            Logger::error("Resized failed");
        }
        buttonRenderTexture.clear(sf::Color::Transparent);
        // std::unique_ptr<sf::Texture> staticTower = tower->getIcon();
        
        // sf::Sprite towerSprite(*(staticTower.get()));
        sf::Sprite towerSprite = tower->getIcon();

        // if (towerSprite.getTexture() == nullptr) {
        //     Logger::error(std::format("Tower {} has no valid texture for icon", towerName));
        //     continue; // Skip this tower if no valid texture
        // }

        towerSprite.setPosition(sf::Vector2f{20.f, 20.f});
        Logger::debug(std::format("{} {}", towerSprite.getLocalBounds().size.x,
                                  towerSprite.getLocalBounds().size.y));
        // towerSprite.setScale({32.f / towerSprite.getLocalBounds().size.x,
        //                       32.f / towerSprite.getLocalBounds().size.y});
        // towerSprite.setScale({1.f, 1.f});

        towerSprite.setOrigin(towerSprite.getLocalBounds().position +
                              towerSprite.getLocalBounds().size / 2.f);

        // int scrapCost = cost["scrap"];
        // int petroleumCost = cost["petroleum"];
        int scrapCost = tower->getCost().getScraps().value;
        int petroleumCost = tower->getCost().getPetroleum().value;

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
        petroleumIcon.setOrigin(petroleumIcon.getLocalBounds().position +
                                petroleumIcon.getLocalBounds().size / 2.f);

        scrapCostDisplay.setOrigin(
            {0.f, scrapCostDisplay.getLocalBounds().position.y +
                      scrapCostDisplay.getLocalBounds().size.y / 2.f});
        petroleumCostDisplay.setOrigin(
            {0.f, petroleumCostDisplay.getLocalBounds().position.y +
                      petroleumCostDisplay.getLocalBounds().size.y / 2.f});
        int yOffset = 35;
        petroleumCostDisplay.setPosition(
            {30, towerSprite.getPosition().y + yOffset + 35});
        petroleumIcon.setPosition(
            {15, towerSprite.getPosition().y + yOffset + 35});
        scrapCostDisplay.setPosition(
            {30, towerSprite.getPosition().y + yOffset + 5});
        scrapIcon.setPosition({15, towerSprite.getPosition().y + yOffset + 5});

        sf::RectangleShape background;
        background.setSize(sf::Vector2f(buttonRenderTexture.getSize() - sf::Vector2u{4, 4}));
        background = Aligner::align(background);
        background.setPosition(static_cast<sf::Vector2f>(buttonRenderTexture.getSize()) / 2.f);
        background.setOutlineThickness(2);
        background.setFillColor(sf::Color::Transparent);
        background.setOutlineColor(sf::Color(0, 0, 0, 255));
        buttonRenderTexture.draw(background);
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

        std::unique_ptr<RectangularButton> button =
            builder.reset()
                .loadJson(buttonStyleID)
                .setBackground(combinedTowerTextures.back().get())
                .setPosition({buttonPosition})
                .setSize(buttonSize)
                .setCallback([this, towerSprite,
                              entry](RectangularButton* button) {
                    Logger::debug("Button presseed");
                    notify("tower_button_pressed", towerName, towerSprite);
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
        level->notify("place_tower_cursor", 0, worldPosition);
        
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
        for (std::unique_ptr<RectangularButton>& button : towerButtons)
            if (button->onMouseEvent(mouse, event, worldPosition,
                                     windowPosition))
                return true;

        if (isTowerSelected &&
            Scaler::screenScale(baseRectangle.getGlobalBounds())
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
        for (std::unique_ptr<RectangularButton>& button : towerButtons) {
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