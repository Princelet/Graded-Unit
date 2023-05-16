#include <fstream>
#include "LevelScreen.h"
#include "Game.h"
#include "Enemy.h"
#include "Block.h"
#include "AttackBox.h"

LevelScreen::LevelScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, player()
	, gameRunning(true)
	, waveCount(7)
	, enemyCount(1)
	, enemyNo(0)
	, spawnTimer(1000)
	, window(newGamePointer->GetWindow())
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

		for (size_t i = 0; i < enemies.size(); ++i)
		{
			enemies[i]->Update(frameTime);
		}

		for (size_t i = 0; i < blocks.size(); ++i)
		{
			blocks[i]->Update(frameTime);
			blocks[i]->SetColliding(false);

			if (blocks[i]->CheckCollision(player))
			{
				player.SetColliding(true);
				blocks[i]->SetColliding(true);
				player.HandleCollision(*blocks[i]);
			}
		}

		for (size_t i = 0; i < enemies.size(); ++i)
		{
			enemies[i]->Update(frameTime);
			enemies[i]->SetColliding(false);

			if (enemies[i]->CheckCollision(player))
			{
				player.SetColliding(true);
				enemies[i]->SetColliding(true);
				player.HandleCollision(*enemies[i]);
				enemies[i]->HandleCollision(player);
			}

			if (enemies[i]->CheckCollision(player.GetAttackBox()))
			{
				player.GetAttackBox().SetColliding(true);
				enemies[i]->SetAlive(0);
			}
		}




		if (enemyCount > 0)
		{

			// Check if timer is 
			if (spawnTimer > 0)
			{
				// Spawn the new enemy into the array
				enemies.push_back(new Enemy(window, this));
				enemies[enemyNo]->Spawn();

				enemyNo++;
			}
			else
			{
				spawnTimer = (rand() % 1000) + 200;
			}
			--enemyCount;
		}
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
	player.GetAttackBox().Draw(target);
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
	enemyNo = 0;

	enemies.clear();
	blocks.clear();

	gameRunning = true;


	// Choose random number of blocks
	int blockCount = (rand() % 10) + 2;
	if (waveCount > 20)
	{
		blockCount += (rand() % 10);
	}

	// Keep obstacles out of the introductory waves
	if (waveCount > 5)
	{
		if (blocks.size() < blockCount)
		{
			for (int i = 0; i < blockCount; ++i)
			{
				blocks.push_back(new Block);
			}
			for (size_t i = 0; i < blocks.size(); ++i)
			{
				blocks[i]->Spawn();
			}
		}
	}


	// Hardcoded enemy counts
	// If anything else, figure it out
	if (waveCount == 1)
	{
		enemyCount = 1;
	}
	else if (waveCount == 2 || waveCount == 3 || waveCount == 4 || waveCount == 6)
	{
		enemyCount = 2;
	}
	else if (waveCount == 5 || waveCount == 7)
	{
		enemyCount = 3;
	}
	else if (waveCount == 8 || waveCount == 9)
	{
		enemyCount = 4;
	}
	else
	{
		// Get half the wave count in order to slowly increase difficulty from this point
		// Rounding doesn't matter because it only makes a 1 enemy difference
		enemyCount = waveCount / 2;
	}
}