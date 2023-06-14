#include <fstream>
#include <iostream>
#include "LevelScreen.h"
#include "Game.h"
#include "Enemy.h"
#include "Block.h"
#include "AttackBox.h"

LevelScreen::LevelScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, gamePointer(newGamePointer)
	, player()
	, gameRunning(true)
	, waveCount(0)
	, enemyCount(1)
	, enemyNo(0)
	, window(newGamePointer->GetWindow())
	, currEnemies(1)
	, endPanel(newGamePointer->GetWindow(), "Game Over", "Unedited Message!")
	, waveText()
	, timerText()
	, healthText()
	, enemyText()
	, enemyInterval(sf::seconds(3))
	, waveDuration(sf::seconds(40))
	, timePerFrame(sf::seconds(0.25f))
	, blockCount(0)
	, endDelay(2000)
{
	// Setup text and objects
	AssetManager::SetupText(waveText, "GameFont", sf::Color::Yellow, "Wave X");
	AssetManager::SetupText(timerText, "GameFont", sf::Color::Yellow, "Time: ");
	AssetManager::SetupText(healthText, "GameFont", sf::Color::Yellow, "HP: ");
	AssetManager::SetupText(enemyText, "GameFont", sf::Color::Yellow, "Enemies: ");

	waveText.setPosition((window->getSize().x / 2.0f) - (waveText.getLocalBounds().width / 2.0f) - 20.0f, 75.0f);
	timerText.setPosition(window->getSize().x - 300.0f, 60.0f);
	healthText.setPosition(20.0f, 160.0f);
	enemyText.setPosition(20.0f, 60.0f);

	waveText.setOutlineThickness(2.0f);
	timerText.setOutlineThickness(2.0f);
	healthText.setOutlineThickness(2.0f);
	enemyText.setOutlineThickness(2.0f);

	rectangle.setPosition(window->getSize().x / 2, window->getSize().y / 2 + 100.0f);
	rectangle.setSize(sf::Vector2f(window->getSize().x - 400.0f, window->getSize().y - 300.0f));
	rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
	rectangle.setFillColor(sf::Color(255, 255, 255, 100));

	banner.setTexture(AssetManager::RequestTexture("TopUI"));
	banner.scale(4.0f, 4.0f);
	banner.setPosition(window->getSize().x / 2, banner.getTexture()->getSize().y / 2 + 100.0f);
	banner.setOrigin(banner.getTexture()->getSize().x / 2, banner.getTexture()->getSize().y / 2);

	player.SetPosition(window->getSize().x / 2, window->getSize().y / 2);

	// Stream Music
	gameMusic.openFromFile("Assets/Audio/ingame.wav");
	gameMusic.setVolume(25);
	gameMusic.setLoop(true);
}

void LevelScreen::Update(sf::Time frameTime)
{
	if (gameRunning)
	{
		float waveTimeFloat = waveTimer.getElapsedTime().asSeconds();
		float remainingTimeFloat = waveDuration.asSeconds() - waveTimeFloat;
		timerText.setString("Time: " + std::to_string((int)ceil(remainingTimeFloat)));

		if (remainingTimeFloat <= 0)
		{
			GameOver();
		}

		float enemyTimeFloat = enemySpawnClock.getElapsedTime().asSeconds();
		float enemyIntervalFloat = enemyInterval.asSeconds() - enemyTimeFloat;

		waveText.setString("Wave " + std::to_string(waveCount));
		healthText.setString("HP: " + std::to_string(player.GetHealth()));
		enemyText.setString("Enemies: " + std::to_string(currEnemies));

		player.Update(frameTime, window);
		player.GetAttackBox().Update(frameTime, window);
		player.SetColliding(false);


		sf::Time timePassedThisFrame = animationClock.getElapsedTime();
		if (timePassedThisFrame >= timePerFrame)
		{
			animationClock.restart();
			player.Animate();
			power.Animate();
		}

		if (heals.CheckCollision(player))
		{
			player.SetColliding(true);
			heals.SetColliding(true);
			player.PickUp("health");

			heals.Die();
		}
		if (power.CheckCollision(player))
		{
			player.SetColliding(true);
			power.SetColliding(true);
			player.PickUp("power");

			power.Die();
		}

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

			if (blocks[i]->CheckCollision(heals))
			{
				heals.SetColliding(true);
				blocks[i]->SetColliding(true);
				heals.HandleCollision(*blocks[i]);
			}

		}

		for (size_t i = 0; i < enemies.size(); ++i)
		{
			if (enemies[i])
			{
				enemies[i]->Update(frameTime, window, player.GetPosition());
				enemies[i]->SetColliding(false);

				if (enemies[i]->CheckCollision(player))
				{
					player.SetColliding(true);
					enemies[i]->SetColliding(true);
					enemies[i]->HandleCollision(player);
				}

				if (enemies[i]->CheckCollision(player.GetAttackBox()))
				{
					player.GetAttackBox().SetColliding(true);

					if (enemies[i]->GetDamageCooldown() == 0)
					{
						enemies[i]->ResetDamageCooldown();
						enemies[i]->TakeDamage(player.GetAttack());
					}
				}

				if (player.CheckCollision(enemies[i]->GetAttackBox()))
				{
					enemies[i]->GetAttackBox().SetColliding(true);

					if (player.GetDamageCooldown() == 0)
					{
						player.ResetDamageCooldown();
						player.TakeDamage(enemies[i]->GetAttack());
						if (player.GetHealth() <= 0)
						{
							GameOver();
						}
					}
				}

				if (enemies[i]->GetHealth() <= 0 && enemies[i] != nullptr)
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
			if (enemyIntervalFloat <= 0)
			{
				// Spawn the new enemy into the array
				enemies.push_back(new Enemy(window, this));
				enemies[enemyNo]->Spawn();

				enemyNo++;
				enemySpawnClock.restart();
				--enemyCount;
				enemyInterval = sf::seconds((rand() % 3) + 2);
			}
		}
		if (currEnemies == 0)
		{
			++waveCount;
			NewWave();
		}

		if (player.GetHealth() == 0)
		{
			GameOver();
		}
	}
	else
	{
		if (endDelay > 0)
			--endDelay;

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::I) || sf::Keyboard::isKeyPressed(sf::Keyboard::J) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::L) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::O) || sf::Keyboard::isKeyPressed(sf::Keyboard::P) ||

			sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))

			&& endDelay == 0)
		{
			gamePointer->SwitchScreen();
		}
	}
}

void LevelScreen::Draw(sf::RenderTarget& target)
{
	target.draw(banner);

	if (gameRunning)
	{
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

		heals.Draw(target);
		power.Draw(target);

		player.Draw(target);
		player.GetAttackBox().Draw(target);
		target.draw(player.GetPlayerPower());
	}
	else
	{
		endPanel.Draw(target);
	}

	target.draw(waveText);
	target.draw(timerText);
	target.draw(healthText);
	target.draw(enemyText);
}

sf::RectangleShape LevelScreen::GetArena()
{
	return rectangle;
}

int LevelScreen::GetWaveCount()
{
	return waveCount;
}

bool LevelScreen::GetGameRunning()
{
	return gameRunning;
}

void LevelScreen::Restart()
{
	gameMusic.play();

	waveCount = 1;
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

	if (waveCount > 50)
		GameOver();

	waveTimer.restart();


	// Choose random number of blocks
	int blockCount = (rand() % 15) + 2;
	if (waveCount > 20)
	{
		blockCount += (rand() % 3);
	}
	if (waveCount > 40)
	{
		blockCount += (rand() % 3);
	}

	// Spawn items if the correct waves
	if (waveCount % 5 == 0 && waveCount > 9)
	{
		heals.Spawn();
	}
	if (waveCount % 4 == 0 && waveCount > 9 && !power.GetAlive())
	{
		power.Spawn();
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
	/*
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
	else if (waveCount > 9 && waveCount < 13)
	{
		enemyCount = 10;
	}
	else if (waveCount > 12)
	{
		enemyCount = 9 + (floor)(waveCount / 4);
	}
	else
	{
		// Fallback value
		enemyCount = 1;
	}
	*/
	enemyCount = 1;

	// Quicker spawns later on
	if (waveCount >= 6 && waveCount <= 14)
	{
		enemyInterval = sf::seconds(2.8f);
		waveDuration = sf::seconds(60);
	}
	else if (waveCount >= 15 && waveCount <= 24)
	{
		enemyInterval = sf::seconds(2.6f);
		waveDuration = sf::seconds(80);
	}
	else if (waveCount >= 25 && waveCount <= 34)
	{
		enemyInterval = sf::seconds(2.4f);
		waveDuration = sf::seconds(100);
	}
	else if (waveCount >= 35 && waveCount <= 44)
	{
		enemyInterval = sf::seconds(2.2f);
		waveDuration = sf::seconds(120);
	}
	else if (waveCount >= 45 && waveCount <= 50)
	{
		enemyInterval = sf::seconds(2.0f);
		waveDuration = sf::seconds(140);
	}

	// Current enemy count to detect how many enemies exist
	currEnemies = enemyCount;
}

void LevelScreen::GameOver()
{
	gameMusic.stop();

	endDelay = 2000;
	gameRunning = false;

	// Get High Scores
	endPanel.SetBody(GetHighScores(waveCount - 1));

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
	struct ScoreEntry
	{
		std::string name;
		int wave = 0;
	};

	std::vector<ScoreEntry> scores;
	ScoreEntry newScore;

	// Open the level file
	std::ifstream inFile;
	inFile.open("Assets/HighScores.txt");

	int scoresCount = 0;
	int position = 0;

	bool added = false;

	std::string bodyText;
	std::string savedName = "";
	std::string savedWave = "";
	std::string playerName = "";

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
			savedName = savedName + ch;
		}
		else if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || 
			ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
		{
			savedWave = savedWave + ch;
		}
		else if (ch == '\n')
		{
			// Add name and wave to high score vector
			newScore.name = savedName;
			newScore.wave = std::stoi(savedWave);
			scores.push_back(newScore);

			// Zero them out
			newScore.name = "";
			newScore.wave = 0;
			savedName = "";
			savedWave = "";

			++scoresCount;
			++position;

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

	// Check if the new score needs to be written
	for (int i = 0; i < 3; ++i)
	{
		if (playerWave >= scores[i].wave && added == false)
		{
			char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

			for (int i = 0; i < 3; ++i)
			{
				playerName = playerName + letters[rand() % 26];
			}

			newScore.name = playerName + " ";
			newScore.wave = playerWave;

			scores.insert(scores.begin() + i, newScore);

			added = true;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		bodyText = bodyText + scores[i].name + "- Wave " + std::to_string(scores[i].wave) + "\n";
	}

	if (added == true)
	{
		// Open the level file
		std::ofstream outFile;
		outFile.open("Assets/HighScores.txt");
		
		outFile << scores[0].name + std::to_string(scores[0].wave) + "\n"
			+ scores[1].name + std::to_string(scores[1].wave) + "\n"
			+ scores[2].name + std::to_string(scores[2].wave) + "\n";

		outFile.close();

		// Player only assigned name if they get a leaderboard position
		// If not added, just do plain text instead of normal player name display
		bodyText = bodyText + "\n\nYou are " + playerName + ".\n\nPress any button\nto exit.";
	}
	else
		bodyText = bodyText + "\n\nTry again!\n\nPress any button\nto exit.";

	return bodyText;
}