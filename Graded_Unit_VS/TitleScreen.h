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

    void ResetDelay();

private:
    void Start();

    sf::RenderWindow* window;
    Game* game;

    sf::Sprite background;
    sf::Sprite selectionIcon;

    sf::Sprite titleText;
    sf::Text startText;
    sf::Text quitText;

    sf::Text moveText;
    sf::Text itemText;

    bool currSelected;

    sf::Time delay;
    sf::Time delay2;
    sf::Time timePerFrame;
};