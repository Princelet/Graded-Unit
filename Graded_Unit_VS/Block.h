#pragma once
#include "Object.h"
#include <SFML/Graphics.hpp>

class Block :
    public Object
{
public:
    Block();
    ~Block();

    void Spawn();
};