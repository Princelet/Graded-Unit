#pragma once
#include "Object.h"

class LevelScreen;

class Enemy :
    public Object
{
public:
    Enemy();

    int GetEnemyType();

private:
    LevelScreen* level;
    sf::Vector2f oldPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    int enemyType;
};