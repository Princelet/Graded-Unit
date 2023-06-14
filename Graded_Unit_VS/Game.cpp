#include "Game.h"
#include "Screen.h"
#include "LevelScreen.h"
#include "TitleScreen.h"

Game::Game()
	: window(sf::VideoMode(1920, 1080), "Demon Knight Attack", sf::Style::Titlebar | sf::Style::Close)
	, gameClock()
	, titleScreen(nullptr)
	, levelScreen(nullptr)
	, inLevel(false)
{
	// Window setup
	window.setMouseCursorVisible(false);

	// Setup screens
	titleScreen = new TitleScreen(this);
	levelScreen = new LevelScreen(this);
}

void Game::RunGameLoop()
{
	// Repeat as long as game is running
	while (window.isOpen())
	{
		Update();
		Draw();
		EventHandling();
	}
}

void Game::EventHandling()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		// Close if Escape pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
	}
}

void Game::Update()
{
	sf::Time frameTime = gameClock.restart();

	// Update current screen
	if (inLevel)
	{
		if (levelScreen != nullptr)
			levelScreen->Update(frameTime);
	}
	else
	{
		if (titleScreen != nullptr)
			titleScreen->Update(frameTime);
	}
}

void Game::Draw()
{
	window.clear(sf::Color(20, 20, 50));

	if (inLevel)
	{
		if (levelScreen != nullptr)
			levelScreen->Draw(window);
	}
	else
	{
		if (titleScreen != nullptr)
			titleScreen->Draw(window);
	}

	window.display();
}

void Game::SwitchScreen()
{
	if (inLevel == true)
		inLevel = false;
	else
		inLevel = true;

	if (inLevel)
	{
		levelScreen->Restart();
	}
}

sf::RenderWindow* Game::GetWindow()
{
	return &window;
}