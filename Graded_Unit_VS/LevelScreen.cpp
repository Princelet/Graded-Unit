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
	, currEnemies(1)
	//, endPanel(window, "Game Over", "Unedited Message!")
{
	rectangle.setPosition(window->getSize().x / 2, window->getSize().y / 2 + 100.0f);
	rectangle.setSize(sf::Vector2f(window->getSize().x - 400.0f, window->getSize().y - 300.0f));
	rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
	rectangle.setFillColor(sf::Color::White);

	Restart();
}

void LevelScreen::Update(sf::Time frameTime)
{
	if (gameRunning)
	{
		player.Update(frameTime, window);
		player.GetAttackBox().Update(frameTime, window);
		player.SetColliding(false);

		for (size_t i = 0; i < blocks.size(); ++i)
		{
			blocks[i]->Update(frameTime, window);
			blocks[i]->SetColliding(false);

			if (blocks[i]->CheckCollision(player))
			{
				player.SetColliding(true);
				blocks[i]->SetColliding(true);
				player.HandleCollision(*blocks[i]);
			}

			for (size_t j = 0; j < enemies.size(); ++j)
			{
				if (enemies[j])
				{
					if (blocks[i]->CheckCollision((*enemies[j])))
					{
						(*enemies[j]).SetColliding(true);
						blocks[i]->SetColliding(true);
						(*enemies[j]).HandleCollision(*blocks[i]);
					}
				}
			}
		}

		for (size_t i = 0; i < enemies.size(); ++i)
		{
			if (enemies[i])
			{
				enemies[i]->Update(frameTime, window, player.GetPosition());
				enemies[i]->SetColliding(false);


				if (enemies[i]->CheckCollision(player.GetAttackBox()))
				{
					player.GetAttackBox().SetColliding(true);
					enemies[i]->TakeDamage();
				}

				if (enemies[i]->GetHealth() == 0 && enemies[i] != nullptr)
				{
					delete enemies[i];
					enemies[i] = nullptr;
					--currEnemies;
				}
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

		if (currEnemies == 0)
		{
			++waveCount;
			NewWave();
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
	target.draw(rectangle);

	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i])
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
	gameRunning = true;

	NewWave();
}

void LevelScreen::NewWave()
{
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
	// Current enemy count to detect how many exist
	currEnemies = enemyCount;
}
