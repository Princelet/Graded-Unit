#pragma once
#include "Screen.h"

class Game;

class TitleScreen :
    public Screen
{
public:
    TitleScreen(Game* newGamePointer);

    void Update(sf::Time frameTime);
    void Draw(sf::RenderTarget& target);

private:
    void Start();

    sf::RenderWindow* window;
    Game* game;

    sf::Sprite background;
    sf::Sprite selectionIcon;

    sf::Text titleText;
    sf::Text startText;
    sf::Text infoText;
    sf::Text quitText;

    bool currSelected;
};