#pragma once
#include "Object.h"
#include "AssetManager.h"

class Game;
class LevelScreen;
class Player;

class TopUI :
    public Object
{
public:
	TopUI(Game* newGamePointer);

	void Update(sf::Time frameTime);
	void Draw(sf::RenderTarget& target);

private:

	sf::Text waveText;
	sf::Text healthText;
	sf::Text timerText;

	sf::RenderWindow* window;
	LevelScreen* level;
	Player* player;
};

