#pragma once
#include "Object.h"

class Enemy :
    public Object
{
public:
    Enemy();
    void RandomType();

private:
    sf::Vector2f oldPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    int enemyType;
};