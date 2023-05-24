#pragma once
#include "Object.h"
#include "AttackBox.h"

class LevelScreen;
class Block;

class Enemy :
    public Object
{
public:
    Enemy(sf::RenderWindow* newWindow, LevelScreen* newLevel);

    void Update(sf::Time frameTime, sf::RenderWindow* window, sf::Vector2f playerPos);
    void HandleCollision(Object& otherObj) override;

    int GetEnemyType();
    int GetHealth();
    int GetDamageCooldown();
    void ResetDamageCooldown();

    AttackBox GetAttackBox();

    void Spawn();
    void TakeDamage();
    void Attack(sf::Vector2f playerPos);

private:
    LevelScreen* level;
    sf::Vector2f oldPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    int spawnBounds;
    sf::Clock spawnClock;
    int interval;
    int enemyType;

    int damageCooldown;
    int health;
    int attack;
    int speed;

    AttackBox atkBox;
    int atkTimer;
    int atkCooldown;
    std::string atkDir;
    float atkDistance;

    sf::RenderWindow* window;

    std::vector<sf::Texture> enemyTextures;
};