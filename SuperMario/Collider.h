#pragma once
#include <SFML/Graphics.hpp>

class Collider
{
private:
	sf::RectangleShape& body;

public:
	Collider(sf::RectangleShape& body);
	~Collider();

	void move(float dx, float dy) const;
	bool checkCollision(Collider const& other) const;
	sf::Vector2f getPosition() const;
	sf::Vector2f getHalfSize() const;
};

