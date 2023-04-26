#include "Enemy.h"
#include "LevelScreen.h"

Enemy::Enemy()
	: Object()
	, level(nullptr)
	, oldPosition(1000.0f, 1000.0f)
	, velocity()
	, acceleration(100.0f, 100.0f)
	, enemyType(0)
{
	// 0 = Normal
	// 1 = Slow
	// 2 = Fast

	// Ordered by when they're first introduced

	int currentWave = level->GetWaveCount();

	// Introducing each type at first
	if (currentWave == 1 || currentWave == 2 || currentWave == 3)
	{
		enemyType = 0;
	}
	else if (currentWave == 4 || currentWave == 5)
	{
		enemyType = 1;
	}
	else if (currentWave == 6 || currentWave == 7)
	{
		enemyType = 2;
	}
	else
	{
		// Temporary
		enemyType = 0;

		// TODO - Random Enemy Types Code

		// get a random number between 1 and 10
		// if (number is 1 to 4) summon normal
		// if (number is 5 to 7) summon slow
		// if (number is 8 to 10) summon fast
	
		// allows weighting to the normal enemies to avoid oversaturation of specials
	}

	// Set colour based on assigned type
	if (enemyType == 0)
		sprite.setColor(sf::Color::Cyan);
	if (enemyType == 1)
		sprite.setColor(sf::Color::Red);
	if (enemyType == 2)
		sprite.setColor(sf::Color::Green);
}

int Enemy::GetEnemyType()
{
	return enemyType;
}