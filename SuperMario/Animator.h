#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Frames {
	std::vector<sf::Vector2u> idle;
	std::vector<sf::Vector2u> walking;
	std::vector<sf::Vector2u> jumping;
	std::vector<sf::Vector2u> dragging;
	std::vector<sf::Vector2u> dying;
};

class Animator
{
public:
	enum State {
		IDLE,
		WALKING,
		DRAGGING,
		JUMPING,
		DYING,
	};

	enum Direction {
		LEFT,
		RIGHT,
	};

private:
	sf::RectangleShape* body;
	sf::Vector2u imageCount;
	sf::Vector2u curImage;
	sf::IntRect uvRect;
	State curState;
	Direction curDirection;
	Frames frames;
	unsigned int curFrame;
	float totalTime;
	float switchTime;

public:
	Animator(sf::RectangleShape* body, sf::Texture* texture, Frames& frames, sf::Vector2u imageCount, float switchTime);
	~Animator();
	void update(float deltaTime, float speedPercent);
	void setState(State state);
	State getState() const;
	void setDirection(Direction direction);
	Direction getDirection() const;
	void flip_x();
	void flip_y();
};

