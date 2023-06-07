#include "PowerItem.h"
#include "AssetManager.h"

PowerItem::PowerItem()
	: Object()
	, isAlive(false)
{
	sprite.setTexture(AssetManager::RequestTexture("Item/PowerA"));

	powerAni.push_back(AssetManager::RequestTexture("Item/PowerA"));
	powerAni.push_back(AssetManager::RequestTexture("Item/PowerB"));

	sprite.setScale(0.2f, 0.2f);
	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
	collisionOffset = sf::Vector2f(-21.0f, -22.0f);
	collisionScale = sf::Vector2f(1.0f, 1.0f);
	SetPosition(0.0f, 0.0f);
	sprite.setColor(sf::Color(255, 255, 255, 0));
}

void PowerItem::HandleCollision(Object& otherObj)
{
	sf::Vector2f depth = GetCollisionDepth(otherObj);
	sf::Vector2f newPos = GetPosition();

	if (abs(depth.x) < abs(depth.y))
	{
		// Move in x direction
		newPos.x += depth.x * 1.5f;
	}
	else
	{
		// Move in y direction
		newPos.y += depth.y * 1.5f;
	}

	SetPosition(newPos);
}

void PowerItem::Spawn()
{
	isAlive = true;
	sprite.setColor(sf::Color(255, 255, 255, 255));

	// Randomly get value between 1 and something
	// Place block that correct amount of X space and Y space from screen border 
	// Check it isnt within other end border

	// Define spacing for grid
	const float X_SPACE = 100.0f;
	const float Y_SPACE = 100.0f;
	const float SCREEN_BORDER_X = 200.0f;
	const float SCREEN_BORDER_TOP = 300.0f;

	int xPos = (rand() % 13);
	int yPos = (rand() % 7);

	// Calculate the actual block position
	float x = (X_SPACE * xPos) + SCREEN_BORDER_X;
	float y = (Y_SPACE * yPos) + SCREEN_BORDER_TOP;

	SetPosition(x, y);
}

void PowerItem::Die()
{
	SetPosition(0.0f, 0.0f);
	sprite.setColor(sf::Color(255, 255, 255, 0));
	isAlive = false;
}

void PowerItem::Animate()
{
	if (sprite.getTexture() == &powerAni[0])
		sprite.setTexture(powerAni[1]);
	else
		sprite.setTexture(powerAni[0]);
}

bool PowerItem::GetAlive()
{
	return isAlive;
}

void PowerItem::SetAlive(bool newAlive)
{
	isAlive = newAlive;
}
