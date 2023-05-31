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
	, damageCooldown(0)
	, atkBox(sf::Vector2f(0, 0))
	, atkCooldown(1000)
	, atkDistance(50)
	, atkTimer(10)
{
	enemyTextures.push_back(AssetManager::RequestTexture("EnemyFront"));
	enemyTextures.push_back(AssetManager::RequestTexture("EnemySide"));
	enemyTextures.push_back(AssetManager::RequestTexture("EnemyBack"));

	// TODO - Edit EnemySide to have same width as front and back to avoid stretching
	sprite.setOrigin(enemyTextures[0].getSize().x / 2, enemyTextures[0].getSize().y / 2);
	sprite.setTexture(enemyTextures[0]);
}

void Enemy::Update(sf::Time frameTime, sf::RenderWindow* window, sf::Vector2f playerPos)
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

	CheckEdges(window);

	// TODO - Different enemy AIs
	if (playerPos.x < GetPosition().x)
	{
		acceleration.x = -accel;
	}
	if (playerPos.y < GetPosition().y)
	{
		acceleration.y = -accel;
	}
	if (playerPos.x > GetPosition().x)
	{
		acceleration.x = accel;
	}
	if (playerPos.y > GetPosition().y)
	{
		acceleration.y = accel;
	}

	if (damageCooldown > 0)
	{
		--damageCooldown;

		// Invulnerability
		sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 50));
	}
	else
		sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));

	if (atkCooldown > 0)
	{
		--atkCooldown;
	}
	else
		Attack(playerPos);
}

void Enemy::HandleCollision(Object& otherObj)
{
	sf::Vector2f depth = GetCollisionDepth(otherObj);
	sf::Vector2f newPos = GetPosition();

	if (abs(depth.x) < abs(depth.y))
	{
		// Move in x direction
		newPos.x += depth.x;
		velocity.x = 0;
		acceleration.x = 0;
	}
	else
	{
		// Move in y direction
		newPos.y += depth.y;
		velocity.y = 0;
		acceleration.y = 0;
	}

	SetPosition(newPos);
}

int Enemy::GetEnemyType()
{
	return enemyType;
}

int Enemy::GetHealth()
{
	return health;
}

int Enemy::GetDamageCooldown()
{
	return damageCooldown;
}

void Enemy::ResetDamageCooldown()
{
	damageCooldown = 1000;
}

AttackBox Enemy::GetAttackBox()
{
	return atkBox;
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
		health = 2;
		attack = 2;
		sprite.setColor(sf::Color(20, 255, 150, 255));
		sprite.setScale(0.3f, 0.3f);
		collisionOffset = sf::Vector2f(-37.0f, -51.0f);
		break;

	case 1:
		speed = 1;
		health = 3;
		attack = 3;
		sprite.setColor(sf::Color(255, 60, 60, 255));
		sprite.setScale(0.4f, 0.4f);
		collisionOffset = sf::Vector2f(-48.0f, -66.0f);
		break;

	case 2:
		speed = 3;
		health = 1;
		attack = 1;
		sprite.setColor(sf::Color(20, 150, 255, 255));
		sprite.setScale(0.2f, 0.2f);
		collisionOffset = sf::Vector2f(-24.0f, -34.0f);
		break;
	}

	collisionScale = sf::Vector2f(1.0f, 1.0f);

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

void Enemy::TakeDamage()
{
	--health;
}

void Enemy::Attack(sf::Vector2f playerPos)
{
	if (playerPos.x < GetPosition().x)
	{
		// Upward attack
		atkTimer = 10;
		atkDir = "up";
		atkCooldown = 1000;
	}
	if (playerPos.y < GetPosition().y)
	{
		// Downward attack
		atkTimer = 10;
		atkDir = "down";
		atkCooldown = 1000;
	}
	if (playerPos.x > GetPosition().x)
	{
		// Left attack
		atkTimer = 10;
		atkDir = "left";
		atkCooldown = 1000;
	}
	if (playerPos.y > GetPosition().y)
	{
		// Right attack
		atkTimer = 10;
		atkDir = "right";
		atkCooldown = 1000;
	}

	// Outside so it sticks while it's alive
	if (atkDir == "up")
	{
		atkBox.SetPosition(sf::Vector2f(GetPosition().x, GetPosition().y - atkDistance));
	}
	if (atkDir == "down")
	{
		atkBox.SetPosition(sf::Vector2f(GetPosition().x, GetPosition().y + atkDistance));
	}
	if (atkDir == "left")
	{
		atkBox.SetPosition(sf::Vector2f(GetPosition().x - atkDistance, GetPosition().y));
	}
	if (atkDir == "right")
	{
		atkBox.SetPosition(sf::Vector2f(GetPosition().x + atkDistance, GetPosition().y));
	}

	if (atkTimer > 0)
	{
		--atkTimer;
	}
	else
	{
		atkDir = "none";
		atkBox.SetPosition(sf::Vector2f(0.0f, -0.0f));
	}
}