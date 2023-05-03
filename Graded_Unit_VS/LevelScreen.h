#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "Player.h"
#include "AssetManager.h"

class Game;
class Platform;
class Enemy;

class LevelScreen :
	public Screen
{
public:
	LevelScreen(Game* newGamePointer);

	void Update(sf::Time frameTime);
	void Draw(sf::RenderTarget& target);

	void EnemySpawn();

	void TriggerEndState(bool win);

	int GetWaveCount();

private:
	void Restart();

	Player player;
	std::vector<Enemy*> enemies;
	int enemyCount;

	bool gameRunning;

	int waveCount;
	sf::RenderWindow* window;
};