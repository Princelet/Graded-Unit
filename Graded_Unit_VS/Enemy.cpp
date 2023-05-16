#include "Enemy.h"
#include "LevelScreen.h" 
#include "AttackBox.h"

Enemy::Enemy(sf::RenderWindow* newWindow, LevelScreen* newLevel)
	: Object()
	, window(newWindow)
	, level(newLevel)
	, oldPosition(1000.0f, 1000.0f)
	, velocity()
	, acceleration(100.0f, 100.0f)
	, enemyType(0)
	, spawnBounds(250)
	, spawnClock()
	, interval(3)
	, attack(2)
	, health(1)
	, speed(2)
{
	enemyTextures.push_back(AssetManager::RequestTexture("EnemyFront"));
	enemyTextures.push_back(AssetManager::RequestTexture("EnemySide"));
	enemyTextures.push_back(AssetManager::RequestTexture("EnemyBack"));

	// Set origin and scale
	sprite.setOrigin(enemyTextures[0].getSize().x / 2, enemyTextures[0].getSize().y / 2);
	sprite.setScale(0.25f, 0.25f);
	// TODO - Edit EnemySide to have same width as front and back to avoid stretching
	sprite.setTexture(enemyTextures[0]);

	collisionOffset = sf::Vector2f(-27.0f, -48.0f);
	collisionScale = sf::Vector2f(1.1f, 1.0f);
}

void Enemy::Update(sf::Time frameTime)
{
	const float DRAG = 20.0f;

	// EXPLICIT EULER (FORWARD EULER)

	SetPosition(GetPosition() + velocity * frameTime.asSeconds());
	velocity += acceleration * frameTime.asSeconds();

	// Drag Calculation
	velocity.x -= velocity.x * DRAG * frameTime.asSeconds();
	velocity.y -= velocity.y * DRAG * frameTime.asSeconds();

	float accel = speed * 1000;


	// Update acceleration
	acceleration.x = 0;
	acceleration.y = 0;

	// Select a random side
	int direction = (rand() % 4);

	// Pick x and y using side
	switch (direction)
	{
	case 0:
		// left
		acceleration.x = -accel;

	case 1:
		// right
		acceleration.x = accel;

	case 2:
		// up
		acceleration.y = -accel;

	case 3:
		// down
		acceleration.y = accel;
	}

	// Don't leave the screen
	if (GetPosition().x < 0)
	{
		SetPosition(GetPosition().x + 150.0f, GetPosition().y);
	}
	if (GetPosition().x > (window->getSize().x + spawnBounds))
	{
		SetPosition(GetPosition().x - 150.0f, GetPosition().y);
	}
	if (GetPosition().y < 0)
	{
		SetPosition(GetPosition().x, GetPosition().y + 150.0f);
	}
	if (GetPosition().y > (window->getSize().y + spawnBounds))
	{
		SetPosition(GetPosition().x, GetPosition().y - 150.0f);
	}
}

int Enemy::GetEnemyType()
{
	return enemyType;
}

void Enemy::Spawn()
{
	// 0 = Normal
	// 1 = Slow
	// 2 = Fast

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

	// Set correct stats for enemy type
	switch (enemyType)
	{
	case 0:
		speed = 2;
		health = 1;
		attack = 2;
		sprite.setColor(sf::Color::Cyan);
		sprite.setScale(0.3f, 0.3f);
		break;

	case 1:
		speed = 1;
		health = 2;
		attack = 3;
		sprite.setColor(sf::Color::Red);
		sprite.setScale(0.4f, 0.4f);
		break;

	case 2:
		speed = 3;
		health = 1;
		attack = 1;
		sprite.setColor(sf::Color::Green);
		sprite.setScale(0.2f, 0.2f);
		break;
	}


	// Set edges of arena
	int minX = spawnBounds;
	int minY = spawnBounds;
	int maxX = window->getSize().x - spawnBounds;
	int maxY = window->getSize().y - spawnBounds;

	// Select a random side
	int side = (rand() % 4);
	float x = window->getSize().x / 2;
	float y = window->getSize().y / 2;

	// Pick x and y using side
	switch (side)
	{
	case 0:
		// left
		x = minX;
		y = (rand() % maxY) + minY;
		break;

	case 1:
		// right
		x = maxX;
		y = (rand() % maxY) + minY;
		break;

	case 2:
		// top
		x = (rand() % maxX) + minX;
		y = minY;
		break;

	case 3:
		// bottom
		x = (rand() % maxX) + minX;
		y = maxY;
		break;
	}

	SetPosition(x, y);
}