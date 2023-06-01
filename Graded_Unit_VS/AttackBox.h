#pragma once
#include "Object.h"

class AttackBox :
    public Object
{
public:
    AttackBox(sf::Vector2f newPosition);
    ~AttackBox();

    void SetRotation(std::string dir);
    void SetEnemy(int enemyType);
    void Hide();
    void Show();
    
private:
    std::vector<sf::Texture> textures;
};