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
    , animationClock()
{
    // Placeholder Texture
    sprite.setTexture(AssetManager::RequestTexture("PlayerFront"));
    AssetManager::SetupWalk("Side", playerWalkSide);
    AssetManager::SetupWalk("Down", playerWalkDown);
    AssetManager::SetupWalk("Up", playerWalkUp);
    sprite.setOrigin(AssetManager::RequestTexture("PlayerSide").getSize().x/2, AssetManager::RequestTexture("PlayerSide").getSize().y/2);

    sprite.setScale(0.25f, 0.25f);
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
}

void Player::HandleCollision(Object& otherObj)
{
    const float JUMPSPEED = 800;
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

        // If we collided from above
        if (depth.y < 0)
        {
            velocity.y = -JUMPSPEED;
        }
    }

    SetPosition(newPos);
}

void Player::Animate()
{
    animationClock.restart();
    std::string lastPressed;


    sf::Time timePassedThisFrame = animationClock.getElapsedTime();
    if (timePassedThisFrame >= timePerFrame)
    {
        animationClock.restart();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            sprite.setScale(0.25f, 0.25f);
            if (sprite.getTexture() == &playerWalkDown[0])
                sprite.setTexture(playerWalkDown[1]);
            else
                sprite.setTexture(playerWalkDown[0]);
            lastPressed = "Down";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            sprite.setScale(0.25f, 0.25f);
            if (sprite.getTexture() == &playerWalkUp[0])
                sprite.setTexture(playerWalkUp[1]);
            else
                sprite.setTexture(playerWalkUp[0]);
            lastPressed = "Up";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            sprite.setScale(0.25f, 0.25f);
            if (sprite.getTexture() == &playerWalkSide[0])
                sprite.setTexture(playerWalkSide[1]);
            else
                sprite.setTexture(playerWalkSide[0]);
            lastPressed = "Left";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            sprite.setScale(-0.25f, 0.25f);
            if (sprite.getTexture() == &playerWalkSide[0])
                sprite.setTexture(playerWalkSide[1]);
            else
                sprite.setTexture(playerWalkSide[0]);
            lastPressed = "Right";
        }
        else
        {
            if (lastPressed == "Up")
            {
                sprite.setScale(0.25f, 0.25f);
                sprite.setTexture(AssetManager::RequestTexture("PlayerFront"));
            }
            else if (lastPressed == "Down")
            {
                sprite.setScale(0.25f, 0.25f);
                sprite.setTexture(AssetManager::RequestTexture("PlayerBack"));
            }
            else if (lastPressed == "Left")
            {
                sprite.setScale(0.25f, 0.25f);
                sprite.setTexture(AssetManager::RequestTexture("PlayerSide"));
            }
            else if (lastPressed == "Right")
            {
                sprite.setScale(-0.25f, 0.25f);
                sprite.setTexture(AssetManager::RequestTexture("PlayerSide"));
            }
        }

    }


}

sf::Vector2f Player::GetOldPosition()
{
    return oldPosition;
}

int Player::GetHealth()
{
    return currentHealth;
}

void Player::UpdateAcceleration()
{
    const float ACCEL = 5000;

    // Update acceleration
    acceleration.x = 0;
    acceleration.y = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        acceleration.x = -ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        acceleration.x = ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        acceleration.y = -ACCEL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        acceleration.y = ACCEL;
    }
}