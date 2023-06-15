#include "TitleScreen.h"
#include "Game.h"
#include "AssetManager.h"

TitleScreen::TitleScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, window(newGamePointer->GetWindow())
	, currSelected(true)
	, game(newGamePointer)
	, timePerFrame(sf::seconds(0.01f))
	, delay(sf::seconds(2.0f))
	, delay2(sf::seconds(0.0f))
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
	if (delay2.asSeconds() > 0)
		delay2 -= timePerFrame;
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) || sf::Keyboard::isKeyPressed(sf::Keyboard::J) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::L) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::O) || sf::Keyboard::isKeyPressed(sf::Keyboard::P) ||

			sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9) ||

			sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		{
			if (currSelected == true)
				Start();
			if (currSelected == false)
				(*window).close();
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && delay.asSeconds() == 0)
		{
			currSelected = !currSelected;
			delay = sf::seconds(2.0f);
		}

		if (delay.asSeconds() > 0)
			delay -= timePerFrame;

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

void TitleScreen::ResetDelay()
{
	delay2 = sf::seconds(3.0f);
}

void TitleScreen::Start()
{
	game->SwitchScreen();
}