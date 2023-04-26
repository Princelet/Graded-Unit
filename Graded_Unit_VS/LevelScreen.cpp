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