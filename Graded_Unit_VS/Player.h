#pragma once
#include "Object.h"

class Player :
    public Object
{
public:
    Player();

    void Update(sf::Time frameTime) override;
    void HandleCollision(Object& otherObj) override;
    void Animate();

    sf::Vector2f GetOldPosition();

    int GetHealth();

private:
    void UpdateAcceleration();

    sf::Vector2f oldPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    std::vector<sf::Texture> playerWalkSide;
    std::vector<sf::Texture> playerWalkDown;
    std::vector<sf::Texture> playerWalkUp;
    std::vector<sf::Texture> playerStill;

    int currentHealth;
    int maxHealth;
    int attack;
};