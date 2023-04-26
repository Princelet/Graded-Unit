#include "Enemy.h"

Enemy::Enemy()
	: Object()
	, oldPosition(1000.0f, 1000.0f)
	, velocity()
	, acceleration(100.0f, 100.0f)
	, enemyType(1)
{
	sprite.setColor(sf::Color::Cyan);
}

void Enemy::RandomType()
{

}