#include "LevelScreen.h"
#include "Game.h"

LevelScreen::LevelScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, player()
	, gameRunning(true)
{
	Restart();

	window = (*newGamePointer).GetWindow();
	size_t windowWidth = (*window).getSize().x;

	AssetManager::SetupText(waveCount, "GameFont", "Cyan", "Wave ");
	waveCount.setPosition((windowWidth / 2.0f) - (waveCount.getLocalBounds().width / 2.0f), 75.0f);

	AssetManager::SetupText(healthText, "GameFont", "Cyan", "HP: ");
	healthText.setPosition(75.0f, 75.0f);

	AssetManager::SetupText(timerText, "GameFont", "Cyan", "Time: ");
	timerText.setPosition(windowWidth - 510.0f, 75.0f);
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
	target.draw(waveCount);
	target.draw(healthText);
	target.draw(timerText);

	player.Draw(target);

	// TODO - Enemy Spawn Timers
	/*
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->Draw(target);
	}
	*/
}

void LevelScreen::TriggerEndState(bool win)
{
	gameRunning = false;

	// TODO - End panel
}

void LevelScreen::Restart()
{
	player.SetPosition(500.0f, 500.0f);
	// Delete before clearing!
	/*
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		delete enemies[i];
		enemies[i] = nullptr;
	}

	enemies.clear();
	*/

	gameRunning = true;
}