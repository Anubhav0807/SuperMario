#include "Collider.h"

Collider::Collider(sf::RectangleShape& body) : body(body) {}

Collider::~Collider() {}

void Collider::move(float dx, float dy) const
{
	body.move(dx, dy);
}

// AABB: Axis-aligned minimum bounding box
bool Collider::checkCollision(Collider const& other) const
{
	sf::Vector2f otherPosition = other.getPosition();
	sf::Vector2f otherHalfSize = other.getHalfSize();
	sf::Vector2f thisPosition = getPosition();
	sf::Vector2f thisHalfSize = getHalfSize();

	float deltaX = otherPosition.x - thisPosition.x;
	float deltaY = otherPosition.y - thisPosition.y;
	float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
	float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

	if (intersectX < 0.0f && intersectY < 0.0f)
		return true;
	else
		return false;
}

sf::Vector2f Collider::getPosition() const
{
	return body.getPosition();
}

sf::Vector2f Collider::getHalfSize() const
{
	return body.getSize() / 2.0f;
}
