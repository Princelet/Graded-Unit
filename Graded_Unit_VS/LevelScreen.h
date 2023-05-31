#pragma once
#include <vector>
#include "Screen.h"
#include "Player.h"
#include "AssetManager.h"
#include "EndPanel.h"

class Game;
class Enemy;
class Block;
class EndPanel;

class LevelScreen :
	public Screen
{
public:
	LevelScreen(Game* newGamePointer);

	void Update(sf::Time frameTime);
	void Draw(sf::RenderTarget& target);

	int GetWaveCount();

private:
	void Restart();
	void NewWave();
	void GameOver();

	std::string GetHighScores(int playerWave);
	std::string AddToHighScores(int playerWave, int position);

	Player player;
	EndPanel endPanel;

	std::vector<Enemy*> enemies;
	std::vector<Block*> blocks;

	int blockCount;
	int enemyCount;
	int waveCount;

	int currEnemies;
	int enemyNo;
	int spawnTimer;

	bool gameRunning;

	sf::RenderWindow* window;
	sf::RectangleShape rectangle;
	sf::Sprite banner;
};