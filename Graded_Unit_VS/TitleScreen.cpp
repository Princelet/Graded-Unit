#include "TitleScreen.h"
#include "Game.h"
#include "AssetManager.h"

TitleScreen::TitleScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, window(newGamePointer->GetWindow())
	, currSelected(true)
	, game(newGamePointer)
{
	background.setTexture(AssetManager::RequestTexture("TopUI"));
	background.scale(4.0f, 4.0f);
	background.setPosition(window->getSize().x / 2, background.getTexture()->getSize().y / 2 + 100.0f);
	background.setOrigin(background.getTexture()->getSize().x / 2, background.getTexture()->getSize().y / 2);

	AssetManager::SetupText(titleText, "GameFont", sf::Color::Cyan, "Demon Knight Attack");
	AssetManager::SetupText(startText, "GameFont", sf::Color::Cyan, "Start");
	AssetManager::SetupText(infoText, "GameFont", sf::Color::Cyan, 
		"Fight off waves of attackers and defend your arena!\nMove with the stick and attack in four directions using the buttons!");
	AssetManager::SetupText(quitText, "GameFont", sf::Color::Cyan, "Quit Game");
}

void TitleScreen::Update(sf::Time frameTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) || sf::Keyboard::isKeyPressed(sf::Keyboard::J)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		if (currSelected == true)
			Start();
		if (currSelected == false)
			(*window).close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		!currSelected;
}

void TitleScreen::Draw(sf::RenderTarget& target)
{
	if (currSelected == true)
	{
		selectionIcon.setPosition(300.0f, 300.0f);
	}
	else
		selectionIcon.setPosition(300.0f, 500.0f);

	target.draw(selectionIcon);
}

void TitleScreen::Start()
{
	game->SwitchScreen();
}