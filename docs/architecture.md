# The project's architecture

## Scene management

```mermaid
classDiagram
class SceneManager {
    -window: RenderWindow
    
    +SceneManager(window)
    +registerScene(string, ~SceneType~)
    +changeScene(string)

    +handleEvent(std::optional~sf::Event~)
    +handleInput()
    +render()
    +update()
}
SceneManager --o "1..n" Scene
class Scene {
    -window: RenderWindow

    +handleEvent(std::optional~sf::Event~)
    +handleInput()
    +draw(sf::RenderTarget, sf::RenderStates)
    +update()
}
Scene <|.. MainMenu
Scene <|.. Gameplay
```
