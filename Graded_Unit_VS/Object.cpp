#include "Object.h"
#include "VectorHelper.h"
#include <algorithm>

Object::Object()
	: position(0.0f, 0.0f)
	, colliding(false)
	, collisionOffset(0.0f, 0.0f)
	, collisionScale(1.0f, 1.0f)
	, collisionType(CollisionType::AABB)
	, alive(true)
{
}

Object::~Object()
{
}

void Object::Update(sf::Time frameTime, sf::RenderWindow* window)
{
}

void Object::Draw(sf::RenderTarget& target)
{
	if (!alive)
		return;

	target.draw(sprite);

	bool drawCollider = true;

	// If a collision isn't occurring, the collision circle will be green.
	sf::Color collisionColor = sf::Color::Green;
	if (colliding)
		collisionColor = sf::Color::Red;
	collisionColor.a = 100;

	if (drawCollider)
	{
		switch (collisionType)
		{
		case CollisionType::CIRCLE:
		{
			sf::CircleShape circle;

			sf::Vector2f shapePos = GetCollisionCentre();
			float circleRadius = GetCircleColliderRadius();;
			shapePos.x -= circleRadius;
			shapePos.y -= circleRadius;

			circle.setPosition(shapePos);
			circle.setRadius(circleRadius);

			circle.setFillColor(collisionColor);

			target.draw(circle);
			break;
		}
		case CollisionType::AABB:
		{
			sf::RectangleShape rectangle;
			sf::FloatRect bounds = GetAABB();
			rectangle.setPosition(bounds.left, bounds.top);
			rectangle.setSize(sf::Vector2f(bounds.width, bounds.height));
			rectangle.setFillColor(collisionColor);

			target.draw(rectangle);
			break;
		}
		default:
			break;
		}
	}
}

void Object::CheckEdges(sf::RenderWindow* window)
{
	const float SCREEN_BORDER_X = 200.0f;
	const float SCREEN_BORDER_TOP = 250.0f;
	const float SCREEN_BORDER_BOTTOM = 100.0f;

	if (position.x <= SCREEN_BORDER_X
		|| position.y <= SCREEN_BORDER_TOP
		|| position.x >= window->getSize().x - SCREEN_BORDER_X
		|| position.y >= window->getSize().y - SCREEN_BORDER_BOTTOM)
	{
		SetPosition(oldPosition);
	}
}

sf::Vector2f Object::GetPosition()
{
	return position;
}

void Object::SetPosition(sf::Vector2f newPosition)
{
	position = newPosition;
	sprite.setPosition(position);
}

void Object::SetPosition(float newX, float newY)
{
	SetPosition(sf::Vector2f(newX, newY));
}

bool Object::CheckCollision(Object otherObj)
{
	if (!alive || !otherObj.alive)
		return false;

	switch (collisionType)
	{
	case CollisionType::CIRCLE:
	{
		if (otherObj.collisionType == CollisionType::CIRCLE)
		{
			// Get the vector representing displacement between two circles
			sf::Vector2f displacement = GetCollisionCentre() - otherObj.GetCollisionCentre();

			// Get vector's magnitude: how far the centres are
			float squareDistance = VectorHelper::SquareMagnitude(displacement);

			// Combine that to the combined radii of the circles
			float combinedRadii = GetCircleColliderRadius() + otherObj.GetCircleColliderRadius();

			return squareDistance <= combinedRadii * combinedRadii;
		}
		else
			// Use function to check if the collisions are different. Boolean is set to True if the first object is a circle, and False ifit is a rectangle
			return CheckDifferentCollision(otherObj, true);
	}
	case CollisionType::AABB:
	{
		if (otherObj.collisionType == CollisionType::AABB)
			return GetAABB().intersects(otherObj.GetAABB());
		else
			return CheckDifferentCollision(otherObj, false);
	}
	default:
		return false;
	}
}

void Object::SetColliding(bool newColliding)
{
	colliding = newColliding;
}

bool Object::CheckDifferentCollision(Object otherObj, bool isCircle)
{
	// Handles a circle colliding with a rectangle

	Object* objectA;
	Object* objectB;

	if (isCircle)
	{
		objectA = this;
		objectB = &otherObj;
	}
	else
	{
		objectA = &otherObj;
		objectB = this;
	}

	sf::Vector2f nearestPointToCircle = objectA->GetCollisionCentre(); // Actual circle centre to start with
	sf::FloatRect otherAABB = objectB->GetAABB();
	// Clamp the circle centre to the AABB of the other object
	nearestPointToCircle.x = fmaxf(otherAABB.left, fminf(nearestPointToCircle.x, otherAABB.left + otherAABB.width));
	nearestPointToCircle.y = fmaxf(otherAABB.top, fminf(nearestPointToCircle.y, otherAABB.top + otherAABB.height));

	// Get the vector representing displacement between two circles
	sf::Vector2f displacement = nearestPointToCircle - objectA->GetCollisionCentre();

	// Get vector's magnitude: how far the centres are
	float squareDistance = VectorHelper::SquareMagnitude(displacement);

	float circleRadius = objectA->GetCircleColliderRadius();

	return squareDistance <= circleRadius * circleRadius;
}

sf::Vector2f Object::GetCollisionDepth(Object otherObj)
{
	sf::FloatRect thisAABB = GetAABB();
	sf::FloatRect otherAABB = otherObj.GetAABB();

	sf::Vector2f thisCentre = GetCollisionCentre();
	sf::Vector2f otherCentre = otherObj.GetCollisionCentre();

	sf::Vector2f minDistance;
	minDistance.x = thisAABB.width * 0.5f + otherAABB.width * 0.5f;
	minDistance.y = thisAABB.height * 0.5f + otherAABB.height * 0.5f;

	sf::Vector2f actualDistance = otherCentre - thisCentre;

	if (actualDistance.x < 0)
		minDistance.x = -minDistance.x;
	if (actualDistance.y < 0)
		minDistance.y = -minDistance.y;

	return actualDistance - minDistance;
}

void Object::HandleCollision(Object& otherObj)
{
	// Do Nothing, handled by child
}

void Object::SetAlive(bool newAlive)
{
	alive = newAlive;
}

sf::Vector2f Object::GetCollisionCentre()
{
	sf::Vector2f centre = position;

	sf::FloatRect bounds = sprite.getGlobalBounds();
	centre.x += collisionOffset.x + bounds.width * 0.5f;
	centre.y += collisionOffset.y + bounds.height * 0.5f;

	return centre;
}

float Object::GetCircleColliderRadius()
{
	sf::FloatRect bounds = sprite.getGlobalBounds();
	bounds.width = bounds.width * collisionScale.x;
	bounds.height = bounds.height * collisionScale.y;

	if (bounds.width > bounds.height)
		return bounds.width * 0.5f;
	else
		return bounds.height * 0.5f;
}

sf::FloatRect Object::GetAABB()
{
	sf::FloatRect bounds = sprite.getGlobalBounds();
	bounds.width = bounds.width * collisionScale.x;
	bounds.height = bounds.height * collisionScale.y;

	sf::Vector2f centre = GetCollisionCentre();

	bounds.left = centre.x - bounds.width * 0.5f;
	bounds.top = centre.y - bounds.height * 0.5f;

	return bounds;
}