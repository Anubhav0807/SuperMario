#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include "Global.h"

static const float bounceHeight = 20.0f;

class Block
{
private:
	sf::RectangleShape body;
	sf::Texture* texture;
	sf::IntRect uvRect;
	sf::Vector2f velocity;
	sf::Vector2f initPos;
	bool canBounce;

public:
	Block(sf::Texture* texture, sf::Vector2f size, bool canBounce);
	~Block();
	Collider getCollider();
	bool isBouncing() const;
	void draw(sf::RenderWindow& window) const;
	void update(float deltaTime);
	void bounce();
	void setPosition(int x, int y);
};

