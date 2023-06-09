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
    Hide();
}

AttackBox::~AttackBox()
{
}

void AttackBox::SetRotation(std::string dir)
{
    if (dir == "up")
    {
        collisionOffset = sf::Vector2f(-58.0f, 5.0f);
        collisionScale = sf::Vector2f(0.7, 1.1f);
        sprite.setTexture(textures[0]);
        sprite.setScale(0.25f, -0.25f);
    }
    if (dir == "down")
    {
        collisionOffset = sf::Vector2f(-58.0f, -55.0f);
        collisionScale = sf::Vector2f(0.7f, 1.1f);
        sprite.setTexture(textures[0]);
        sprite.setScale(0.25f, 0.25f);
    }
    if (dir == "left")
    {
        collisionOffset = sf::Vector2f(5.0f, -58.0f);
        collisionScale = sf::Vector2f(1.1f, 0.7f);
        sprite.setTexture(textures[1]);
        sprite.setScale(-0.25f, 0.25f);
    }
    if (dir == "right")
    {
        collisionOffset = sf::Vector2f(-55.0f, -58.0f);
        collisionScale = sf::Vector2f(1.1f, 0.7f);
        sprite.setTexture(textures[1]);
        sprite.setScale(0.25f, 0.25f);
    }
}

void AttackBox::SetEnemy(int enemyType)
{
    if (enemyType == 0)
    {
        sprite.setColor(sf::Color(20, 255, 150, 255));
        sprite.setScale(0.6f, 0.6f);
    }
    if (enemyType == 1)
    {
        sprite.setColor(sf::Color(255, 60, 60, 255));
        sprite.setScale(1.0f, 1.0f);
    }
    if (enemyType == 2)
    {
        sprite.setColor(sf::Color(20, 150, 255, 255));
        sprite.setScale(0.4f, 0.4f);
    }
}

void AttackBox::Hide()
{
    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 0));
}

void AttackBox::Show()
{
    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));
}