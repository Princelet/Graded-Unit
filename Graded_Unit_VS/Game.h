#pragma once
#include <SFML/Graphics.hpp>

class LevelScreen;
class TitleScreen;

class Game
{
public:
    Game();

    void RunGameLoop();
    void EventHandling();
    void Update();
    void Draw();

    void SwitchScreen();

    sf::RenderWindow* GetWindow();

private:
    sf::RenderWindow window;
    sf::Clock gameClock;
    LevelScreen* levelScreen;
    TitleScreen* titleScreen;

    bool inLevel;
};