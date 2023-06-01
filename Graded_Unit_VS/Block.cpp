#include "Block.h"
#include "AssetManager.h"

Block::Block()
	: Object()
{
	sprite.setTexture(AssetManager::RequestTexture("Block"));
	sprite.setScale(3.0f, 3.0f);
	collisionOffset = sf::Vector2f(0.0f, 0.0f);
	collisionScale = sf::Vector2f(1.0f, 1.0f);
	SetPosition(100.0f, 100.0f);
}

Block::~Block()
{
}

void Block::Spawn()
{
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