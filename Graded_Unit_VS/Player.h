#pragma once
#include "Object.h"
#include "AttackBox.h"

class Player :
    public Object
{
public:
    Player();

    void Update(sf::Time frameTime, sf::RenderWindow* window) override;
    void HandleCollision(Object& otherObj) override;

    sf::Vector2f GetOldPosition();
    AttackBox GetAttackBox();
    int GetDamageCooldown();
    void ResetDamageCooldown();

    int GetHealth();
    int GetAttack();
    bool GetHasPower();
    sf::Sprite GetPlayerPower();
    void PickUp(std::string itemName);

    void Attack();
    void TakeDamage(int damage);
    void Animate();

private:
    void UpdateAcceleration();
    
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    std::vector<sf::Texture> playerWalkSide;
    std::vector<sf::Texture> playerWalkDown;
    std::vector<sf::Texture> playerWalkUp;
    std::vector<sf::Texture> playerStill;

    AttackBox atkBox;
    int atkTimer;
    int atkCooldown;
    std::string atkDir;
    float atkDistance;
    int baseAtkCooldown;

    int currentHealth;
    int maxHealth;
    int attack;
    int damageCooldown;

    bool hasPower;
    sf::Sprite power;
    std::vector<sf::Texture> powerAni;
    sf::Clock powerClock;
    sf::Time powerEnd;
};