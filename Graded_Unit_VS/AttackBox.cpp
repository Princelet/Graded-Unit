#include "AttackBox.h"
#include "AssetManager.h"

AttackBox::AttackBox(sf::Vector2f newPosition)
	: Object()
{
    sprite.setTexture(AssetManager::RequestTexture("PlayerConcept"));
    // Set origin and scale
    sprite.setOrigin(sprite.getScale().x / 2, sprite.getScale().y / 2);

    collisionOffset = sf::Vector2f(0.0f, 0.0f);
    collisionScale = sf::Vector2f(1.1f, 0.9f);
    SetPosition(newPosition);
}

AttackBox::~AttackBox()
{
}