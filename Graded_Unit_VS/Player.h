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
    sf::Time GetDamageCooldown();
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
	sf::Time timePerFrame;

    std::vector<sf::Texture> playerWalkSide;
    std::vector<sf::Texture> playerWalkDown;
    std::vector<sf::Texture> playerWalkUp;
    std::vector<sf::Texture> playerStill;

    AttackBox atkBox;
    sf::Time atkTimer;
    sf::Time atkCooldown;
    sf::Time baseAtkCooldown;
    sf::Time damageCooldown;
    std::string atkDir;
    float atkDistance;

    int currentHealth;
    int maxHealth;
    int attack;

    bool hasPower;
    sf::Sprite power;
    std::vector<sf::Texture> powerAni;
    sf::Clock powerClock;
    sf::Time powerEnd;
};