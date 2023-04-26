#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "Player.h"
#include "AssetManager.h"

class Game;
class Platform;

class LevelScreen :
	public Screen
{
public:
	LevelScreen(Game* newGamePointer);

	void Update(sf::Time frameTime);
	void Draw(sf::RenderTarget& target);

	void TriggerEndState(bool win);

private:
	void Restart();

	Player player;
	// std::vector<Enemy*> enemies;
	bool gameRunning;

	sf::Text waveCount;
	sf::Text healthText;
	sf::Text timerText;
	sf::RenderWindow* window;
};