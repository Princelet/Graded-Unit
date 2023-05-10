#pragma once
#include "Object.h"

class LevelScreen;

class Enemy :
    public Object
{
public:
    Enemy(sf::RenderWindow* newWindow, LevelScreen* newLevel);

    void Update(sf::Time frameTime) override;

    int GetEnemyType();

    void Spawn();

private:
    LevelScreen* level;
    sf::Vector2f oldPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    int spawnBounds;
    sf::Clock spawnClock;
    int interval;
    int enemyType;

    int health;
    int attack;
    int speed;

    sf::RenderWindow* window;

    std::vector<sf::Texture> enemyTextures;
};