#include "Enemy.h"
#include "LevelScreen.h"

Enemy::Enemy()
	: Object()
	, level(nullptr)
	, oldPosition(1000.0f, 1000.0f)
	, velocity()
	, acceleration(100.0f, 100.0f)
	, enemyType(0)
	, spawnClock()
	, interval(3)
	, attack(2)
	, health(1)
	, speed(2)
{
	sprite.setTexture(AssetManager::RequestTexture("EnemyFront"));

	// Set origin and scale
	sprite.setOrigin(AssetManager::RequestTexture("EnemySide").getSize().x / 2, AssetManager::RequestTexture("EnemySide").getSize().y / 2);
	sprite.setScale(0.25f, 0.25f);

	// TODO - Edit EnemySide to have same width as front and back to avoid stretching

}

int Enemy::GetEnemyType()
{
	return enemyType;
}

void Enemy::Spawn(float newX, float newY)
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
	// If it isn't a starting wave...
	else
	{
		// Get a random number between 1 and 10
		int typeNo = (rand() % 10);

		// Find which kind we spawn 
		if (typeNo >= 5 && typeNo < 8)
		{
			// Slow enemy
			enemyType = 1;
		}
		else if (typeNo >= 8 && typeNo < 11)
		{
			// Fast enemy
			enemyType = 2;
		}
		else
		{
			// Weighted to normal enemies to keep balance
			// Also means if value is somehow out of bounds we just spawn a normal
			enemyType = 0;
		}
	}


	switch (enemyType)
	{
	case 0:
		speed = 2;
		health = 1;
		attack = 2;
		sprite.setColor(sf::Color::Cyan);
		sprite.setScale(0.25f, 0.25f);
		break;

	case 1:
		speed = 1;
		health = 2;
		attack = 3;
		sprite.setColor(sf::Color::Red);
		sprite.setScale(0.5f, 0.5f);
		break;

	case 2:
		speed = 3;
		health = 1;
		attack = 1;
		sprite.setColor(sf::Color::Green);
		sprite.setScale(0.1f, 0.1f);
		break;
	}

	sprite.setPosition(newX, newY);
}
