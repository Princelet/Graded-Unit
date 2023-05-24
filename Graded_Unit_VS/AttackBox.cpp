#include "AttackBox.h"
#include "AssetManager.h"

AttackBox::AttackBox(sf::Vector2f newPosition)
	: Object()
{
    textures.push_back(AssetManager::RequestTexture("AttackBoxVert"));
    textures.push_back(AssetManager::RequestTexture("AttackBoxHori"));
    // Set origin and scale
    sprite.setOrigin(sprite.getScale().x / 2, sprite.getScale().y / 2);
    sprite.setScale(0.25f, 0.25f);
    sprite.setRotation(180.0f);
    SetPosition(newPosition);
}

AttackBox::~AttackBox()
{
}

void AttackBox::SetRotation(std::string dir)
{
    if (dir == "up")
    {
        collisionOffset = sf::Vector2f(-58.0f, -10.0f);
        collisionScale = sf::Vector2f(0.7, 1.3f);
        sprite.setTexture(textures[0]);
        sprite.setScale(0.25f, -0.25f);
    }
    if (dir == "down")
    {
        collisionOffset = sf::Vector2f(-58.0f, -50.0f);
        collisionScale = sf::Vector2f(0.7f, 1.3f);
        sprite.setTexture(textures[0]);
        sprite.setScale(0.25f, 0.25f);
    }
    if (dir == "left")
    {
        collisionOffset = sf::Vector2f(-10.0f, -58.0f);
        collisionScale = sf::Vector2f(1.3f, 0.7f);
        sprite.setTexture(textures[1]);
        sprite.setScale(-0.25f, 0.25f);
    }
    if (dir == "right")
    {
        collisionOffset = sf::Vector2f(-50.0f, -58.0f);
        collisionScale = sf::Vector2f(1.3f, 0.7f);
        sprite.setTexture(textures[1]);
        sprite.setScale(0.25f, 0.25f);
    }
}