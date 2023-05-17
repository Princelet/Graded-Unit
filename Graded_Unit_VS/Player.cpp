#include "Player.h"
#include "AssetManager.h"

enum class PhysicsType
{
    FORWARD_EULER,
    BACKWARD_EULER
};

Player::Player()
    : Object()
    , oldPosition(100.0f, 300.0f)
    , velocity()
    , acceleration(100.0f, 100.0f)
    , currentHealth(20)
    , maxHealth(30)
    , attack(1)
    , powerCounter(0)
    , hasShield(false)
    , atkTimer(10)
    , atkDistance(50)
    , atkBox(sf::Vector2f(0,0))
    , atkCooldown(0)
{
    // Starting texture
    sprite.setTexture(AssetManager::RequestTexture("PlayerFront"));

    // Assign still textures for easy access
    playerStill.push_back(AssetManager::RequestTexture("PlayerFront"));
    playerStill.push_back(AssetManager::RequestTexture("PlayerBack"));
    playerStill.push_back(AssetManager::RequestTexture("PlayerSide"));
    
    // Set up walking animations
    AssetManager::SetupWalk("Side", playerWalkSide);
    AssetManager::SetupWalk("Down", playerWalkDown);
    AssetManager::SetupWalk("Up", playerWalkUp);

    // Set origin and scale
    sprite.setOrigin(playerStill[0].getSize().x/2, playerStill[0].getSize().y/2);
    sprite.setScale(0.25f, 0.25f);

    collisionOffset = sf::Vector2f(-27.0f, -48.0f);
    collisionScale = sf::Vector2f(1.1f, 1.0f);
}

void Player::Update(sf::Time frameTime)
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

    Animate();
    Attack();

    // Power item countdown
    if (attack < 1)
    {
        --powerCounter;
    }
}

void Player::HandleCollision(Object& otherObj)
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

sf::Vector2f Player::GetOldPosition()
{
    return oldPosition;
}

AttackBox Player::GetAttackBox()
{
    return atkBox;
}

int Player::GetHealth()
{
    return currentHealth;
}

void Player::PickUp(std::string itemName)
{
    // Activate correct ability for item
    if (itemName == "health" && currentHealth != maxHealth)
    {
        currentHealth += 5;
    }
    if (itemName == "power")
    {
        attack = 9;
        powerCounter = 1000;
    }
    if (itemName == "shield")
    {
        hasShield = true;
    }
}

void Player::Attack()
{
    if (atkCooldown == 0)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            // Upward attack
            atkTimer = 10;
            atkDir = "up";
            atkCooldown = 1000;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        {
            // Downward attack
            atkTimer = 10;
            atkDir = "down";
            atkCooldown = 1000;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            // Left attack
            atkTimer = 10;
            atkDir = "left";
            atkCooldown = 1000;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            // Right attack
            atkTimer = 10;
            atkDir = "right";
            atkCooldown = 1000;
        }
    }
    else
    {
        --atkCooldown;
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
}

void Player::UpdateAcceleration()
{
    const float ACCEL = 5200;

    // Update acceleration
    acceleration.x = 0;
    acceleration.y = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        acceleration.x = -ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        acceleration.x = ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        acceleration.y = -ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        acceleration.y = ACCEL;
    }
}