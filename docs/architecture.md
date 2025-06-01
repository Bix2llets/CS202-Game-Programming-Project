# The project's architecture

## Scene management

```mermaid
classDiagram
class SceneManager {
    -window: RenderWindow
    
    +SceneManager(window)
    +registerScene~SceneType~(string)
    +changeScene(string)

    +handleEvent(std::optional~sf::Event~)
    +handleInput()
    +render()
    +update()
}
SceneManager --o "1..n" Scene
class Scene {
    -window: RenderWindow

    +handleEvent(std::optional~sf::Event~)*
    +handleInput()*
    +draw(sf::RenderTarget, sf::RenderStates)*
    +update()*
}
Scene <|.. MainMenu
Scene <|.. Gameplay

class MainMenu {
    +handleEvent(std::optional~sf::Event~)
    +handleInput()
    +draw(sf::RenderTarget, sf::RenderStates)
    +update()
}
class Gameplay {
    +handleEvent(std::optional~sf::Event~)
    +handleInput()
    +draw(sf::RenderTarget, sf::RenderStates)
    +update()
}
```

## Entities

```mermaid
classDiagram
class Interactive {
    handleEvent(sf::RenderWindow, std::optional~sf::Event~ event)*
    handleInput(sf::RenderWindow)*
}

class GameObject {
    draw()*
    update()*

}
```