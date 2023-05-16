#include "AttackBox.h"
#include "AssetManager.h"

AttackBox::AttackBox(sf::Vector2f newPosition)
	: Object()
{
    sprite.setTexture(AssetManager::RequestTexture("PlayerConcept"));
    // Set origin and scale
    sprite.setOrigin(sprite.getScale().x / 2, sprite.getScale().y / 2);
    sprite.setScale(0.25f, 0.25f);

    collisionOffset = sf::Vector2f(-27.0f, -48.0f);
    collisionScale = sf::Vector2f(1.1f, 1.0f);
    SetPosition(newPosition);
}

AttackBox::~AttackBox()
{
}

void AttackBox::HandleCollision(Object& otherObj)
{
    /*
    if (otherObj.sprite.getTexture() = AssetManager::RequestTexture("EnemyFront"))
    {
        delete otherObj;
    }
    */
}