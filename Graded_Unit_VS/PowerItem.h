#pragma once
#include "Object.h"
class PowerItem :
    public Object
{
public:
    PowerItem();

    void HandleCollision(Object& otherObj) override;
    void Spawn();
    void Die();

    void Animate();

    bool GetAlive();
    void SetAlive(bool newAlive);

private:
    std::vector<sf::Texture> powerAni;
    bool isAlive;
};

