#include "TitleScreen.h"
#include "Game.h"
#include "AssetManager.h"

TitleScreen::TitleScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, window(newGamePointer->GetWindow())
	, currSelected(true)
	, game(newGamePointer)
	, delay(1000)
	, delay2(2000)
{
	// Set Textures
	// Temp BG texture
	background.setTexture(AssetManager::RequestTexture("TopUI"));
	background.scale(4.0f, 4.0f);
	background.setPosition(window->getSize().x / 2, background.getTexture()->getSize().y / 2 + 100.0f);
	background.setOrigin(background.getTexture()->getSize().x / 2, background.getTexture()->getSize().y / 2);

	selectionIcon.setTexture(AssetManager::RequestTexture("Block"));
	selectionIcon.scale(0.6f, 0.6f);
	selectionIcon.setOrigin(selectionIcon.getTexture()->getSize().x / 2, selectionIcon.getTexture()->getSize().y / 2);

	titleText.setTexture(AssetManager::RequestTexture("Title"));
	titleText.setScale(0.4f, 0.4f);
	titleText.setPosition((window->getSize().x / 4.0f), 300.0f);
	titleText.setOrigin(titleText.getTexture()->getSize().x / 2, titleText.getTexture()->getSize().y / 2);


	AssetManager::SetupText(startText, "GameFont", sf::Color::Cyan, "Start");
	AssetManager::SetupText(moveText, "GameFont", sf::Color::Yellow, 
		"Fight off waves of attackers from all sides!\nMove with the stick and attack in four directions using the buttons!");
	AssetManager::SetupText(itemText, "GameFont", sf::Color::Yellow,
		"Your allies will drop you hearts and attack boosters!\nThe enemy mage will summon blocks in your way. Use them to your advantage!");
	AssetManager::SetupText(quitText, "GameFont", sf::Color::Cyan, "Quit Game");

	startText.setPosition(1100.0f, window->getSize().y / 2 - 10.0f);
	quitText.setPosition(1100.0f, window->getSize().y / 2 + 130.0f);

	moveText.setScale(0.6f, 0.6f);
	moveText.setPosition(150.0f, window->getSize().y - 220.0f);
	moveText.setLineSpacing(2.0f);
	itemText.setScale(0.5f, 0.5f);
	itemText.setPosition(210.0f, window->getSize().y - 140.0f);
	itemText.setLineSpacing(2.0f);
}

void TitleScreen::Update(sf::Time frameTime)
{
	if (delay2 > 0)
		--delay2;
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) || sf::Keyboard::isKeyPressed(sf::Keyboard::J)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		{
			if (currSelected == true)
				Start();
			if (currSelected == false)
				(*window).close();
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && delay == 0)
		{
			currSelected = !currSelected;
			delay = 2000;
		}

		if (delay > 0)
			--delay;

		if (currSelected == true)
		{
			selectionIcon.setPosition(900.0f, window->getSize().y / 2 + 10.0f);
		}
		else
			selectionIcon.setPosition(900.0f, window->getSize().y / 2 + 150.0f);
	}
}

void TitleScreen::Draw(sf::RenderTarget& target)
{
	target.draw(background);

	target.draw(titleText);
	target.draw(startText);
	target.draw(moveText);
	target.draw(itemText);
	target.draw(quitText);

	target.draw(selectionIcon);
}

void TitleScreen::Start()
{
	delay2 = 2000;
	game->SwitchScreen();
}