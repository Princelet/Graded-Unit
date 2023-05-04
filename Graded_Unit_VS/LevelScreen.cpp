#include <fstream>
#include "LevelScreen.h"
#include "Game.h"
#include "Enemy.h"
#include "Block.h"

LevelScreen::LevelScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, player()
	, gameRunning(true)
	, waveCount(10)
	, enemyCount(1)
	//, endPanel(window, "Game Over", "Unedited Message!")
{
	Restart();
}

void LevelScreen::Update(sf::Time frameTime)
{
	if (gameRunning)
	{
		player.Update(frameTime);
		player.SetColliding(false);
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
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		blocks[i]->Draw(target);
	}

	player.Draw(target);
}

void LevelScreen::EnemySpawn(int enemyNo)
{
	int currentEnemy = enemyNo - 1;

	float windowX = (*window).getSize().x;
	float windowY = (*window).getSize().y;

	int maxY = windowY - 20;
	int minY = windowY + 20;
	int maxX = windowX - 20;
	int minX = windowX + 20;

	int side = (rand() % 4);
	int x = 0;
	int y = 0;

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
	enemies[currentEnemy]->Spawn(x, y);
}

void LevelScreen::BlockSpawn(int newBlockCount)
{
	// Define spacing for grid
	const float X_SPACE = 50.0f;
	const float Y_SPACE = 50.0f;
	const float SCREEN_BORDER = 200.0f;

	// Keep obstacles out of the introductory waves
	if (waveCount > 5)
	{
		if (blocks.size() < blockCount)
		{
			// Randomly get value between 1 and something
			// Place block that correct amount of X space and Y space from screen border 
			// Check it isnt within other end border

			for (int i = 0; i < blockCount; ++i)
			{
				int xPos = (rand() % 10);
				int yPos = (rand() % 10);

				for (int i = 0; i < blockCount; ++i)
				{
					do {
						xPos = (rand() % 10);
						yPos = (rand() % 10);
					} while (xPos == blocks[i]->GetPosition().x && yPos == blocks[i]->GetPosition().y);
				}

				// Calculate the actual block position
				float blockPosX = (X_SPACE * xPos) + SCREEN_BORDER;
				float blockPosY = (Y_SPACE * yPos) + SCREEN_BORDER;

				blocks.push_back(new Block(sf::Vector2f(blockPosX, blockPosY)));
			}
		}
	}
}


void LevelScreen::TriggerEndState(bool win)
{
	gameRunning = false;
	//endPanel.StartAnimation();
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
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		delete blocks[i];
		blocks[i] = nullptr;
	}

	enemies.clear();
	blocks.clear();

	gameRunning = true;


	// Choose random number of blocks
	int blockCount = (rand() % 10) + 2;
	if (waveCount > 20)
	{
		blockCount += (rand() % 10);
	}

	BlockSpawn(blockCount);


	// Get enemy info from file
	std::ifstream inFile;
	inFile.open("Assets/EnemyData/EarlyLevelEnemies.txt");

	// Read each character one by one...
	char ch;

	while (inFile >> std::noskipws >> ch)
	{
		// TODO - Setup enemy count read in based on line
		enemyCount = 3; // TEMP VALUE
	}

	// Close the file since we're done
	inFile.close();
}