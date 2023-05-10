#include "Block.h"
#include "AssetManager.h"

Block::Block(sf::Vector2f newPosition)
	: Object()
{
	sprite.setTexture(AssetManager::RequestTexture("PlayerConcept"));
	collisionOffset = sf::Vector2f(0, -57.0f);
	collisionScale = sf::Vector2f(1.0f, 0.25f);
	SetPosition(newPosition);
}

Block::~Block()
{
}