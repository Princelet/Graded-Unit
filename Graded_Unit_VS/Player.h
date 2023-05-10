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
    void PickUp(std::string itemName);

    void Attack();

private:
    void Animate();
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

    int powerCounter;
    bool hasShield;

    sf::Vector2f atkPos;
    float atkDistance;
};