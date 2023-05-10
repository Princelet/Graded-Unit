#pragma once
#include "Object.h"

class AttackBox :
    public Object
{
public:
    AttackBox(sf::Vector2f newPosition);
    ~AttackBox();

    void Draw();
    void Update();
    void HandleCollision(Object& otherObj) override;

    void Die();

private:
    int lifeTime;
};

