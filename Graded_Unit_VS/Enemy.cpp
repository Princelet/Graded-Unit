#include "Enemy.h"
#include "LevelScreen.h" 
#include "AttackBox.h"

Enemy::Enemy(sf::RenderWindow* newWindow, LevelScreen* newLevel)
	: Object()
	, window(newWindow)
	, level(newLevel)
	, velocity()
	, acceleration(100.0f, 100.0f)
	, enemyType(0)
	, spawnBounds(250)
	, spawnClock()
	, interval(3)
	, attack(2)
	, health(1)
	, speed(2)
	, damageCooldown(10)
	, atkBox(sf::Vector2f(0, 0))
	, atkCooldown(1000)
	, atkDistance(50)
	, atkTimer(10)
	, baseAtkCooldown(1500)
{
	enemyTextures.push_back(AssetManager::RequestTexture("EnemyFront"));
	enemyTextures.push_back(AssetManager::RequestTexture("EnemySide"));
	enemyTextures.push_back(AssetManager::RequestTexture("EnemyBack"));

	// TODO - Edit EnemySide to have same width as front and back to avoid stretching
	sprite.setOrigin(enemyTextures[0].getSize().x / 2, enemyTextures[0].getSize().y / 2);
	sprite.setTexture(enemyTextures[0]);

	oldPosition = sf::Vector2f(newLevel->GetArena().getSize().x - (rand() % 1000), newLevel->GetArena().getSize().y - 200.0f);
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

int Enemy::GetAttack()
{
	return attack;
}

int Enemy::GetDamageCooldown()
{
	return damageCooldown;
}

void Enemy::ResetDamageCooldown()
{
	damageCooldown = 500;
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
		sprite.setScale(0.26f, 0.26f);
		collisionOffset = sf::Vector2f(-32.0f, -45.0f);
		baseAtkCooldown = 650;
		break;

	case 1:
		speed = 1;
		health = 3;
		attack = 3;
		sprite.setColor(sf::Color(255, 60, 60, 255));
		sprite.setScale(0.32f, 0.32f);
		collisionOffset = sf::Vector2f(-38.0f, -56.0f);
		baseAtkCooldown = 800;
		break;

	case 2:
		speed = 3;
		health = 1;
		attack = 1;
		sprite.setColor(sf::Color(20, 150, 255, 255));
		sprite.setScale(0.2f, 0.2f);
		collisionOffset = sf::Vector2f(-24.0f, -34.0f);
		baseAtkCooldown = 500;
		break;
	}
	
	atkBox.SetEnemy(enemyType);

	collisionScale = sf::Vector2f(1.0f, 1.0f);

	// Set edges of arena
	int minX = spawnBounds;
	int minY = spawnBounds;
	int maxX = (*level).GetArena().getSize().x - spawnBounds;
	int maxY = (*level).GetArena().getSize().y - spawnBounds;

	// Select a random side
	int side = (rand() % 4);
	float x = (*level).GetArena().getSize().x / 2;
	float y = (*level).GetArena().getSize().y / 2;

	// Pick x and y using side
	switch (side)
	{
	case 0:
		// left
		x = minX - 180;
		y = (rand() % maxY) + minY;
		break;

	case 1:
		// right
		x = maxX + 180;
		y = (rand() % maxY) + minY;
		break;

	case 2:
		// top
		x = (rand() % maxX) + minX;
		y = minY + 180;
		break;

	case 3:
		// bottom
		x = (rand() % maxX) + minX;
		y = maxY - 180;
		break;
	}

	SetPosition(x, y);
}

void Enemy::TakeDamage(int damage)
{
	health -= damage;
}

void Enemy::Attack(sf::Vector2f playerPos)
{
	if (playerPos.x < GetPosition().x)
	{
		// Upward attack
		atkTimer = 10;
		atkDir = "up";
		atkCooldown = baseAtkCooldown;
		atkBox.SetRotation("up");
		atkBox.Show();
	}
	if (playerPos.y < GetPosition().y)
	{
		// Downward attack
		atkTimer = 10;
		atkDir = "down";
		atkCooldown = baseAtkCooldown;
		atkBox.SetRotation("down");
		atkBox.Show();
	}
	if (playerPos.x > GetPosition().x)
	{
		// Left attack
		atkTimer = 10;
		atkDir = "left";
		atkCooldown = baseAtkCooldown;
		atkBox.SetRotation("left");
		atkBox.Show();
	}
	if (playerPos.y > GetPosition().y)
	{
		// Right attack
		atkTimer = 10;
		atkDir = "right";
		atkCooldown = baseAtkCooldown;
		atkBox.SetRotation("right");
		atkBox.Show();
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
		atkBox.Hide();
	}
}