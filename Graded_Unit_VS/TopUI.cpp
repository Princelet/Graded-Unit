#include "TopUI.h"
#include "Game.h"
#include "LevelScreen.h"

TopUI::TopUI(Game* newGamePointer)
	: Object()
	, window((*newGamePointer).GetWindow())
	, level(nullptr)
	, player(nullptr)
	, waveText()
	, healthText()
	, timerText()
{
	size_t windowWidth = (*window).getSize().x;

	AssetManager::SetupText(waveText, "GameFont", "Cyan", "Wave ");
	waveText.setPosition((windowWidth / 2.0f) - (waveText.getLocalBounds().width / 2.0f), 75.0f);

	AssetManager::SetupText(healthText, "GameFont", "Cyan", "HP: ");
	healthText.setPosition(75.0f, 75.0f);

	AssetManager::SetupText(timerText, "GameFont", "Cyan", "Time: ");
	timerText.setPosition(windowWidth - 510.0f, 75.0f);
}

void TopUI::Update(sf::Time frameTime)
{
	waveText.setString("Wave " + (*level).GetWaveCount());
	healthText.setString("HP: " + (*player).GetHealth());
	// TODO - Timer Text
}

void TopUI::Draw(sf::RenderTarget& target)
{
	target.draw(waveText);
	target.draw(healthText);
	target.draw(timerText);
}
