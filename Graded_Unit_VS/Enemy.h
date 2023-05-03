#pragma once
#include "Object.h"

class LevelScreen;

class Enemy :
    public Object
{
public:
    Enemy();

    int GetEnemyType();

    void Spawn(float newX, float newY);

private:
    LevelScreen* level;
    sf::Vector2f oldPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    sf::Clock spawnClock;
    float interval;
    int enemyType;

    int health;
    int attack;
    int speed;
};