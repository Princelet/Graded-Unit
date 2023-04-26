#include "Game.h"
#include "Screen.h"
#include "LevelScreen.h"

Game::Game()
	: window(sf::VideoMode::getDesktopMode(), "Demon Knight Attack", sf::Style::Titlebar | sf::Style::Close)
	, gameClock()
	, currentScreen(nullptr)
{
	// Window setup
	window.setMouseCursorVisible(false);

	// TODO: Setup screens
	currentScreen = new LevelScreen(this);
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
	if (currentScreen != nullptr)
		currentScreen->Update(frameTime);
}

void Game::Draw()
{
	window.clear(sf::Color(20, 20, 50));

	if (currentScreen != nullptr)
		currentScreen->Draw(window);

	window.display();
}

sf::RenderWindow* Game::GetWindow()
{
	return &window;
}