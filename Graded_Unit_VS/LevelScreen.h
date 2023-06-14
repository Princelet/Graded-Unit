#pragma once
#include <vector>
#include "Screen.h"
#include "Player.h"
#include "AssetManager.h"
#include "EndPanel.h"
#include "HealthItem.h"
#include "PowerItem.h"

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

	sf::RectangleShape GetArena();
	int GetWaveCount();
	bool GetGameRunning();
	void Restart();

private:
	void NewWave();
	void GameOver();

	Game* gamePointer;

	std::string GetHighScores(int playerWave);

	Player player;
	EndPanel endPanel;

	std::vector<Enemy*> enemies;
	std::vector<Block*> blocks;
	HealthItem heals;
	PowerItem power;

	int blockCount;
	int enemyCount;
	int waveCount;

	int currEnemies;
	int enemyNo;

	bool gameRunning;
	int endDelay;

	sf::RenderWindow* window;
	sf::RectangleShape rectangle;
	sf::Sprite banner;

	sf::Text waveText;
	sf::Text timerText;
	sf::Text healthText;
	sf::Text enemyText;


	sf::Clock waveTimer;
	sf::Clock enemySpawnClock;
	sf::Clock animationClock;

	sf::Time enemyInterval;
	sf::Time waveDuration;
	sf::Time timePerFrame;

	sf::Music gameMusic;
};