#pragma once
#include "Object.h"

class HealthItem :
    public Object
{
public:
    HealthItem();

    void HandleCollision(Object& otherObj) override;
    void Spawn();
};