#pragma once
#include <SFML/Graphics.hpp>

enum class CollisionType
{
	CIRCLE,
	AABB
};

class Object
{
public:
	Object();
	~Object();

	virtual void Update(sf::Time frameTime, sf::RenderWindow* window);
	virtual void Draw(sf::RenderTarget& target);
	virtual void CheckEdges(sf::RenderWindow* window);

	sf::Vector2f GetPosition();
	virtual void SetPosition(sf::Vector2f newPosition);
	void SetPosition(float newX, float newY);

	bool CheckCollision(Object otherObj);
	void SetColliding(bool newColliding);
	bool CheckDifferentCollision(Object otherObj, bool isCircle);

	sf::Vector2f GetCollisionDepth(Object otherObj);
	virtual void HandleCollision(Object& otherObj);

	void SetAlive(bool newAlive);

protected:
	sf::Sprite sprite;

	sf::Vector2f collisionOffset;
	sf::Vector2f collisionScale;

	CollisionType collisionType;

	bool alive;
	sf::Vector2f oldPosition;

private:
	sf::Vector2f position;
	sf::Vector2f GetCollisionCentre();
	float GetCircleColliderRadius();
	sf::FloatRect GetAABB();

	bool colliding;
};