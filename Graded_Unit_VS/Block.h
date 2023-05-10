#pragma once
#include "Object.h"
#include <SFML/Graphics.hpp>

class Block :
    public Object
{
public:
    Block(sf::Vector2f newPosition);
    ~Block();

private:
    sf::Vector2f position;
};