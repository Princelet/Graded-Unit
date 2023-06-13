#include "TitleScreen.h"
#include "Game.h"
#include "AssetManager.h"

TitleScreen::TitleScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, window(newGamePointer->GetWindow())
	, currSelected(true)
	, game(newGamePointer)
{
	// Set Textures
	// Temp BG texture
	background.setTexture(AssetManager::RequestTexture("TopUI"));
	background.scale(4.0f, 4.0f);
	background.setPosition(window->getSize().x / 2, background.getTexture()->getSize().y / 2 + 100.0f);
	background.setOrigin(background.getTexture()->getSize().x / 2, background.getTexture()->getSize().y / 2);

	selectionIcon.setTexture(AssetManager::RequestTexture("Block"));
	selectionIcon.scale(0.5f, 0.5f);
	selectionIcon.setOrigin(selectionIcon.getTexture()->getSize().x / 2, selectionIcon.getTexture()->getSize().y / 2);


	AssetManager::SetupText(titleText, "GameFont", sf::Color::Cyan, "Demon Knight Attack");
	AssetManager::SetupText(startText, "GameFont", sf::Color::Cyan, "Start");
	AssetManager::SetupText(infoText, "GameFont", sf::Color::Cyan, 
		"Fight off waves of attackers and defend your arena!\nMove with the stick and attack in four directions using the buttons!");
	AssetManager::SetupText(storyText, "GameFont", sf::Color::Cyan,
		"They'll be attacking from all sides!\nRemember to watch out for their witch's obstacles!");
	AssetManager::SetupText(quitText, "GameFont", sf::Color::Cyan, "Quit Game");

	titleText.setScale(1.5f, 1.5f);
	titleText.setPosition((window->getSize().x / 2.0f) - (titleText.getLocalBounds().width / 2.0f) - 20.0f, 100.0f);
	startText.setPosition(600.0f, window->getSize().y / 2 - 10.0f);
	quitText.setPosition(600.0f, window->getSize().y / 2 + 130.0f);

	infoText.setScale(0.5f, 0.5f);
	infoText.setPosition(150.0f, window->getSize().y - 180.0f);
	infoText.setLineSpacing(2.0f);
	storyText.setScale(0.5f, 0.5f);
	storyText.setPosition(200.0f, window->getSize().y - 120.0f);
	storyText.setLineSpacing(2.0f);
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

	if (currSelected == true)
	{
		selectionIcon.setPosition(500.0f, window->getSize().y / 2 - 10.0f);
	}
	else
		selectionIcon.setPosition(500.0f, window->getSize().y / 2 + 130.0f);
}

void TitleScreen::Draw(sf::RenderTarget& target)
{
	target.draw(background);

	target.draw(titleText);
	target.draw(startText);
	target.draw(infoText);
	target.draw(storyText);
	target.draw(quitText);

	target.draw(selectionIcon);
}

void TitleScreen::Start()
{
	game->SwitchScreen();
}