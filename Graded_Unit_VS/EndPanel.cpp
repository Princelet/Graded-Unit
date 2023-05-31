#include "EndPanel.h"
#include "AssetManager.h"
#include "Easing.h"

EndPanel::EndPanel(sf::RenderWindow* newWindow, std::string titleText, std::string messageText)
	: Object()
	, background()
	, title()
	, message()
	, window(newWindow)
	, animatingIn(false)
	, animationClock()
	, xPos()
{
	background.setTexture(AssetManager::RequestTexture("GlassPanel"));
	title.setFont(AssetManager::RequestFont("GameFont"));
	message.setFont(AssetManager::RequestFont("GameFont"));

	background.setScale(4, 3);

	title.setCharacterSize(70);
	title.setString(titleText);
	title.setOutlineColor(sf::Color::Black);
	title.setOutlineThickness(10);

	message.setCharacterSize(30);
	message.setString(messageText);
	message.setLineSpacing(1.5);

	ResetPosition();
}

void EndPanel::Update(sf::Time frameTime)
{
	if (animatingIn)
	{
		float yPos = window->getSize().y;
		float finalYPos = (window->getSize().y - background.getGlobalBounds().height) * 0.5f;

		sf::Vector2f begin(xPos, yPos);
		sf::Vector2f change(0, finalYPos - yPos);
		float duration = 1.2f;
		float time = animationClock.getElapsedTime().asSeconds();

		SetPosition(Easing::QuadIn(begin, change, duration, time));

		if (time >= duration)
		{
			SetPosition(begin + change);
			animatingIn = false;
		}
	}
}

void EndPanel::Draw(sf::RenderTarget& target)
{
	target.draw(background);
	target.draw(message);
	target.draw(title);
}

void EndPanel::SetPosition(sf::Vector2f newPosition)
{
	background.setPosition(newPosition);

	float titleX = (background.getGlobalBounds().width - title.getGlobalBounds().width) * 0.5f;
	title.setPosition(newPosition.x + titleX, newPosition.y + 30);
	float messageX = (background.getGlobalBounds().width - message.getGlobalBounds().width) * 0.5f;
	message.setPosition(newPosition.x + messageX, newPosition.y + 150);
}

void EndPanel::StartAnimation(bool win)
{
	animatingIn = true;
	animationClock.restart();
}

void EndPanel::ResetPosition()
{
	xPos = (window->getSize().x - background.getGlobalBounds().width) * 0.5f;
	float yPos = (window->getSize().y - background.getGlobalBounds().height) * 0.5f;
	SetPosition(sf::Vector2f(xPos, yPos));
}

void EndPanel::SetBody(std::string bodyText)
{
	message.setString(bodyText);
}