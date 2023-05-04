#pragma once
#include "Object.h"
#include <SFML/Graphics.hpp>

class Block :
    public Object
{
public:
    Block(sf::Vector2f newPosition);
    ~Block();

    sf::Vector2f GetPosition();

private:
    sf::Vector2f position;
};