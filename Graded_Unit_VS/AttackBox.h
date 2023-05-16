#pragma once
#include "Object.h"

class AttackBox :
    public Object
{
public:
    AttackBox(sf::Vector2f newPosition);
    ~AttackBox();

    void HandleCollision(Object& otherObj) override;
};

