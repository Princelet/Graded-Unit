#include <fstream>
#include "LevelScreen.h"
#include "Game.h"
#include "Enemy.h"

LevelScreen::LevelScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, player()
	, gameRunning(true)
	, waveCount(1)
	, enemyCount(1)
{
	Restart();
}

void LevelScreen::Update(sf::Time frameTime)
{
	if (gameRunning)
	{
		player.Update(frameTime);
		player.SetColliding(false);

		// TODO - Arena Bounds
		/*
		if (player.GetPosition().y > window.getSize().y ||
			player.GetPosition().y < 0 ||
			player.GetPosition().x > window.getSize().x ||
			player.GetPosition().x < 0)
		{
			player.SetPosition(player.GetOldPosition());
		}
		*/
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			Restart();
	}

}

void LevelScreen::Draw(sf::RenderTarget& target)
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->Draw(target);
	}

	player.Draw(target);

}

void LevelScreen::EnemySpawn()
{
	// Get enemy info from file
	std::ifstream inFile;
	inFile.open("Assets/EnemyData/EarlyLevelEnemies.txt");

	// Delete before clearing!
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		delete enemies[i];
		enemies[i] = nullptr;
	}

	enemies.clear();


	// Read each character one by one...
	char ch;

	while (inFile >> std::noskipws >> ch)
	{
		// TODO - Setup enemy count read in based on line
		enemyCount = 1; // TEMP VALUE
	}

	// Close the file since we're done
	inFile.close();

	float windowX = (*window).getSize().x;
	float windowY = (*window).getSize().y;

	int maxY = windowY - 20;
	int minY = windowY + 20;
	int maxX = windowX - 20;
	int minX = windowX + 20;

	// For every enemy needed in this level, pick a location to spawn
	for (size_t i = 0; i < enemyCount; ++i)
	{
		int side = (rand() % 4);
		float x = 0;
		float y = 0;

		switch (side)
		{
		case 0:
			// left
			x = minX;
			y = (rand() % maxY) + minY;
			break;

		case 1:
			// right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;

		case 2:
			// top
			x = (rand() % maxX) + minX;
			y = minY;
			break;

		case 3:
			// bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}

		// Spawn the new enemy into the array
		enemies[i]->Spawn(x, y);
	}
}


void LevelScreen::TriggerEndState(bool win)
{
	gameRunning = false;

	// TODO - End panel
}

int LevelScreen::GetWaveCount()
{
	return waveCount;
}

void LevelScreen::Restart()
{
	player.SetPosition(500.0f, 500.0f);
	// Delete before clearing!
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		delete enemies[i];
		enemies[i] = nullptr;
	}

	enemies.clear();

	gameRunning = true;
}