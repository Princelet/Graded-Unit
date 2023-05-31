#include <fstream>
#include <iostream>
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
	, endPanel(newGamePointer->GetWindow(), "Game Over", "Unedited Message!")
{
	rectangle.setPosition(window->getSize().x / 2, window->getSize().y / 2 + 100.0f);
	rectangle.setSize(sf::Vector2f(window->getSize().x - 400.0f, window->getSize().y - 300.0f));
	rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
	rectangle.setFillColor(sf::Color::White);

	banner.setTexture(AssetManager::RequestTexture("TopUI"));
	banner.scale(4.0f, 4.0f);
	banner.setPosition(window->getSize().x / 2, banner.getTexture()->getSize().y / 2 + 100.0f);
	banner.setOrigin(banner.getTexture()->getSize().x / 2, banner.getTexture()->getSize().y / 2);

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
					
					if (enemies[i]->GetDamageCooldown() == 0)
					{
						enemies[i]->ResetDamageCooldown();
						enemies[i]->TakeDamage();
					}
				}

				if (player.CheckCollision(enemies[i]->GetAttackBox()))
				{
					enemies[i]->GetAttackBox().SetColliding(true);

					if (player.GetDamageCooldown() == 0)
					{
						player.ResetDamageCooldown();
						player.TakeDamage();
					}
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

	if (player.GetHealth() == 0)
	{
		GameOver();
	}
}

void LevelScreen::Draw(sf::RenderTarget& target)
{
	target.draw(banner);
	target.draw(rectangle);

	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i])
		{
			enemies[i]->Draw(target);
			enemies[i]->GetAttackBox().Draw(target);
		}
	}
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		blocks[i]->Draw(target);
	}

	player.Draw(target);
	player.GetAttackBox().Draw(target);
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


	if (waveCount <= 50)
	{
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
	else
	{
		GameOver();
	}
}

void LevelScreen::GameOver()
{
	gameRunning = false;

	// Get High Scores
	endPanel.SetBody(GetHighScores(waveCount));

	if (waveCount > 50)
	{
		endPanel.StartAnimation(true);
	}
	else
	{
		endPanel.StartAnimation(false);
	}
}

std::string LevelScreen::GetHighScores(int playerWave)
{
	// Open the level file
	std::ifstream inFile;
	inFile.open("Assets/HighScores.txt");

	int scoresCount = 0;
	int position = 0;
	bool startWaveCount = true;

	std::string bodyText = "";

	std::string savedWave;

	// Make sure the file was actually opened
	if (!inFile)
	{
		// File was not opened
		return "Scores Could Not Be Read";
	}

	// Read each character one by one...
	char ch;

	// The "noskipws" means we include white space (spaces/newlines)
	while (inFile >> std::noskipws >> ch)
	{
		// Perform action based on what was read in
		if (ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E' || ch == 'F' || ch == 'G' || 
			ch == 'H' || ch == 'I' || ch == 'J' || ch == 'K' || ch == 'L' || ch == 'M' || ch == 'N' || 
			ch == 'O' || ch == 'P' || ch == 'Q' || ch == 'R' || ch == 'S' || ch == 'T' || ch == 'U' || 
			ch == 'V' || ch == 'W' || ch == 'X' || ch == 'Y' || ch == 'Z' || ch == ' ')
		{
			bodyText = bodyText + ch;
		}
		else if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || 
			ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
		{
			if (startWaveCount)
			{
				bodyText = bodyText + "Wave";
				startWaveCount = false;
			}

			bodyText = bodyText + ch;

			// TODO - Print Wave Number
			savedWave = savedWave + ch;
			if (waveCount > std::stoi(savedWave))
			{
				bodyText = AddToHighScores(waveCount, position);
			}
		}
		else if (ch == '\n')
		{
			bodyText = bodyText + ch;

			++scoresCount;
			++position;

			startWaveCount = true;
			savedWave = "";

			if (scoresCount == 3)
			{
				inFile.close();
			}
		}
		else
		{
			std::cerr << "Unrecognised character in level file: " << ch;
		}
	}

	// Close the file since we're done
	inFile.close();

	return bodyText;
}

std::string LevelScreen::AddToHighScores(int playerWave, int position)
{
	// TODO - Get Player Name (3 characters)
	char playerName1 = 'A';
	char playerName2 = 'A';
	char playerName3 = 'A';
	// i want to do this but how do i do a letter wheel
	



	// ----------------------
	//
	// TODO - MAKE PLAYER PUT IN NAME ON START-UP!
	// SOLVES EVERYTHING IM WORKING ON HERE
	//
	// ----------------------



	// Turn the wave counter into individual digits
	int digit1 = (waveCount / 1) % 10;
	int digit2 = (waveCount / 10) % 10;

	int linePos = 0;

	std::string newBody;

	// Open the level file
	std::ifstream inFile;
	inFile.open("Assets/HighScores.txt");

	char ch;
	while (inFile.get(ch))
	{
		// 
		newBody.push_back(ch);
		if (ch == '\n')
		{
			++linePos;

			// If in the correct place, add the new score
			if (linePos == position)
				newBody.push_back(playerName1 + playerName2 + playerName3 + ' ' + digit1 + digit2);

			// If there are three scores, stop printing them
			if (linePos > 3)
				break;
		}
	}

	// Close the file since we're done
	inFile.close();

	return newBody;
}