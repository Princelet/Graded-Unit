#pragma once
#include "Object.h"

class Player :
    public Object
{
public:
    Player();

    void Update(sf::Time frameTime) override;
    void HandleCollision(Object& otherObj) override;

    sf::Vector2f GetOldPosition();

    int GetHealth();

private:
    void UpdateAcceleration();

    sf::Vector2f oldPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    int currentHealth;
    int maxHealth;
    int attack;
};