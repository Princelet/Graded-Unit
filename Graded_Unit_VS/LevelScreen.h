#pragma once
#include <vector>
#include "Screen.h"
#include "Player.h"
#include "AssetManager.h"
// #include "EndPanel.h"

class Game;
class Enemy;
class Block;

class LevelScreen :
	public Screen
{
public:
	LevelScreen(Game* newGamePointer);

	void Update(sf::Time frameTime);
	void Draw(sf::RenderTarget& target);

	void BlockSpawn(int newBlockCount);
	void TriggerEndState(bool win);

	int GetWaveCount();

private:
	void Restart();

	Player player;
	// EndPanel endPanel;

	std::vector<Enemy*> enemies;
	std::vector<Block*> blocks;

	int blockCount;
	int enemyCount;
	int waveCount;

	int enemyNo;
	int spawnTimer;

	bool gameRunning;

	sf::RenderWindow* window;
};