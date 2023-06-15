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
    int GetAttack();
    sf::Time GetDamageCooldown();
    void ResetDamageCooldown();

    AttackBox GetAttackBox();

    void Spawn();
    void TakeDamage(int damage);
    void Attack(sf::Vector2f playerPos);

private:
    LevelScreen* level;

    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Time timePerFrame;

    int spawnBounds;
    sf::Clock spawnClock;
    sf::Time interval;
    int enemyType;

    int health;
    int attack;
    int speed;

    AttackBox atkBox;
    sf::Time atkTimer;
    sf::Time atkCooldown;
    sf::Time damageCooldown;
    sf::Time baseAtkCooldown;
    std::string atkDir;
    float atkDistance;

    sf::RenderWindow* window;

    std::vector<sf::Texture> enemyTextures;
};