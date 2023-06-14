#include "Player.h"
#include "AssetManager.h"

enum class PhysicsType
{
    FORWARD_EULER,
    BACKWARD_EULER
};

Player::Player()
    : Object()
    , velocity()
    , acceleration(100.0f, 100.0f)
    , currentHealth(10)
    , maxHealth(20)
    , attack(1)
    , atkTimer(100)
    , atkDistance(100)
    , atkBox(sf::Vector2f(0, 0))
    , atkCooldown(0)
    , damageCooldown(0)
    , baseAtkCooldown(600)
    , hasPower(false)
    , powerEnd(sf::seconds(5.0f))
{
    // Starting texture
    sprite.setTexture(AssetManager::RequestTexture("PlayerFront"));

    // Assign still textures for easy access
    playerStill.push_back(AssetManager::RequestTexture("PlayerFront"));
    playerStill.push_back(AssetManager::RequestTexture("PlayerBack"));
    playerStill.push_back(AssetManager::RequestTexture("PlayerSide"));

    // Assign textures to the power animation
    power.setTexture(AssetManager::RequestTexture("Item/PowerA"));
    powerAni.push_back(AssetManager::RequestTexture("Item/PowerA"));
    powerAni.push_back(AssetManager::RequestTexture("Item/PowerB"));

    power.setOrigin(power.getTexture()->getSize().x / 2, power.getTexture()->getSize().y / 2);
    power.setScale(0.15f, 0.15f);
    
    // Set up walking animations
    AssetManager::SetupWalk("Side", playerWalkSide);
    AssetManager::SetupWalk("Down", playerWalkDown);
    AssetManager::SetupWalk("Up", playerWalkUp);

    // Set origin and scale
    sprite.setOrigin(playerStill[0].getSize().x/2, playerStill[0].getSize().y/2);
    sprite.setScale(0.25f, 0.25f);

    collisionOffset = sf::Vector2f(-27.0f, -48.0f);
    collisionScale = sf::Vector2f(1.1f, 1.0f);

    oldPosition = sf::Vector2f(100.0f, 300.0f);
}

void Player::Update(sf::Time frameTime, sf::RenderWindow* window)
{
    const float DRAG = 20.0f;
    const PhysicsType physics = PhysicsType::FORWARD_EULER;
    sf::Vector2f lastFramePos = GetPosition();

    switch (physics)
    {
    case PhysicsType::FORWARD_EULER:
    {
        // EXPLICIT EULER (FORWARD EULER)

        SetPosition(GetPosition() + velocity * frameTime.asSeconds());
        velocity += acceleration * frameTime.asSeconds();

        // Drag Calculation
        velocity.x -= velocity.x * DRAG * frameTime.asSeconds();
        velocity.y -= velocity.y * DRAG * frameTime.asSeconds();

        // Update Acceleration
        UpdateAcceleration();

        break;
    }

    case PhysicsType::BACKWARD_EULER:
    {
        // IMPLICIT EULER (BACKWARD EULER)

        // Update Acceleration
        UpdateAcceleration();

        velocity += acceleration * frameTime.asSeconds();

        // Drag Calculation
        velocity.x -= velocity.x * DRAG;
        velocity.y -= velocity.y * DRAG;

        SetPosition(GetPosition() + velocity * frameTime.asSeconds());

        break;
    }
    default:
        break;
    }

    // Two frames ago (on next frame)
    oldPosition = lastFramePos;

    CheckEdges(window);
    Attack();

    // Power item countdown
    float powerTimeFloat = powerClock.getElapsedTime().asSeconds();
    if (powerTimeFloat > powerEnd.asSeconds())
    {
        hasPower = false;
    }

    if (hasPower)
    {
        power.setPosition(GetPosition().x, GetPosition().y + 20.0f);
        power.setColor(sf::Color(255, 255, 255, 196));
        sprite.setColor(sf::Color(160, 255, 160, sprite.getColor().a));
    }
    else
    {
        sprite.setColor(sf::Color(255, 255, 255, sprite.getColor().a));
        power.setColor(sf::Color(255, 255, 255, 0));
        power.setPosition(0.0f, 0.0f);
    }

    if (damageCooldown > 0)
    {
        --damageCooldown;

        // Invulnerability
        sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 50));
    }
    else
        sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 255));

    if (currentHealth == 0)
    {
        sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, 0));
    }
}

void Player::HandleCollision(Object& otherObj)
{
    sf::Vector2f depth = GetCollisionDepth(otherObj);
    sf::Vector2f newPos = GetPosition();

    if (abs(depth.x) < abs(depth.y))
    {
        // Move in x direction
        newPos.x += depth.x * 2.0f;
        velocity.x = 0;
        acceleration.x = 0;
    }
    else
    {
        // Move in y direction
        newPos.y += depth.y * 2.0f;
        velocity.y = 0;
        acceleration.y = 0;
    }

    SetPosition(newPos);
}

sf::Vector2f Player::GetOldPosition()
{
    return oldPosition;
}

AttackBox Player::GetAttackBox()
{
    return atkBox;
}

int Player::GetDamageCooldown()
{
    return damageCooldown;
}

void Player::ResetDamageCooldown()
{
    damageCooldown = 400;
}

int Player::GetHealth()
{
    return currentHealth;
}

int Player::GetAttack()
{
    if (hasPower)
    {
        return 5;
    }
    else
        return 1;
}

bool Player::GetHasPower()
{
    return hasPower;
}

sf::Sprite Player::GetPlayerPower()
{
    return power;
}

void Player::PickUp(std::string itemName)
{
    // Activate correct ability for item
    if (itemName == "health" && currentHealth != maxHealth)
    {
        currentHealth += 2;
    }
    if (itemName == "power")
    {
        hasPower = true;
        powerClock.restart();
    }
}

void Player::Attack()
{
    if (atkCooldown <= 0)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) || sf::Keyboard::isKeyPressed(sf::Keyboard::O) || sf::Keyboard::isKeyPressed(sf::Keyboard::P) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
        {
            // Upward attack
            atkTimer = 10;
            atkDir = "up";
            atkCooldown = baseAtkCooldown;
            atkBox.SetRotation(atkDir);
            atkBox.Show();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
        {
            // Downward attack
            atkTimer = 10;
            atkDir = "down";
            atkCooldown = baseAtkCooldown;
            atkBox.SetRotation(atkDir);
            atkBox.Show();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            // Left attack
            atkTimer = 10;
            atkDir = "left";
            atkCooldown = baseAtkCooldown;
            atkBox.SetRotation(atkDir);
            atkBox.Show();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
        {
            // Right attack
            atkTimer = 10;
            atkDir = "right";
            atkCooldown = baseAtkCooldown;
            atkBox.SetRotation(atkDir);
            atkBox.Show();
        }
    }
    else
    {
        --atkCooldown;
    }

    // Outside so it sticks while it's alive
    if (atkDir == "up")
    {
        atkBox.SetPosition(sf::Vector2f(GetPosition().x + 30.0f, GetPosition().y - atkDistance));
    }
    if (atkDir == "down")
    {
        atkBox.SetPosition(sf::Vector2f(GetPosition().x + 30.0f, GetPosition().y + atkDistance));
    }
    if (atkDir == "left")
    {
        atkBox.SetPosition(sf::Vector2f(GetPosition().x - atkDistance, GetPosition().y + 20.0f));
    }
    if (atkDir == "right")
    {
        atkBox.SetPosition(sf::Vector2f(GetPosition().x + atkDistance, GetPosition().y + 20.0f));
    }

    if (atkTimer > 0)
    {
        --atkTimer;
    }
    else
    {
        atkDir = "none";
        atkBox.SetPosition(sf::Vector2f(-100.0f, -100.0f));
        atkBox.Hide();
    }
}

void Player::TakeDamage(int damage)
{
    currentHealth -= damage;
}

void Player::Animate()
{
    std::string lastPressed;

    // For each direction, set scale and texture
    // If continuously going same direction, alternate the sprite used
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        sprite.setScale(0.25f, 0.25f);
        if (sprite.getTexture() == &playerWalkDown[0])
            sprite.setTexture(playerWalkDown[1]);
        else
            sprite.setTexture(playerWalkDown[0]);
        lastPressed = "Down";
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        sprite.setScale(0.25f, 0.25f);
        if (sprite.getTexture() == &playerWalkUp[0])
            sprite.setTexture(playerWalkUp[1]);
        else
            sprite.setTexture(playerWalkUp[0]);
        lastPressed = "Up";
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        sprite.setScale(0.25f, 0.25f);
        if (sprite.getTexture() == &playerWalkSide[0])
            sprite.setTexture(playerWalkSide[1]);
        else if (sprite.getTexture() == &playerWalkSide[1])
            sprite.setTexture(playerStill[2]);
        else
            sprite.setTexture(playerWalkSide[0]);
        lastPressed = "Left";
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        sprite.setScale(-0.25f, 0.25f);
        if (sprite.getTexture() == &playerWalkSide[0])
            sprite.setTexture(playerWalkSide[1]);
        else if (sprite.getTexture() == &playerWalkSide[1])
            sprite.setTexture(playerStill[2]);
        else
            sprite.setTexture(playerWalkSide[0]);
        lastPressed = "Right";
    }
    else
    {
        // Set Still Sprite and Scale based on last pressed key
        // 0 = Down, 1 = Up, 2 = Side
        if (lastPressed == "Up")
        {
            sprite.setScale(0.25f, 0.25f);
            sprite.setTexture(playerStill[0]);
        }
        else if (lastPressed == "Down")
        {
            sprite.setScale(0.25f, 0.25f);
            sprite.setTexture(playerStill[1]);
        }
        else if (lastPressed == "Left")
        {
            sprite.setScale(0.25f, 0.25f);
            sprite.setTexture(playerStill[2]);
        }
        else if (lastPressed == "Right")
        {
            sprite.setScale(-0.25f, 0.25f);
            sprite.setTexture(playerStill[2]);
        }
    }

    // Power animation
    if (attack > 1)
    {
        if (power.getTexture() == &powerAni[0])
            power.setTexture(powerAni[1]);
        else
            power.setTexture(powerAni[0]);
    }
}

void Player::UpdateAcceleration()
{
    const float ACCEL = 5200;

    // Update acceleration
    acceleration.x = 0;
    acceleration.y = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        acceleration.x = -ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        acceleration.x = ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        acceleration.y = -ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        acceleration.y = ACCEL;
    }
}