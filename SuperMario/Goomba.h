#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include "Block.h"
#include "Animator.h"
#include "Global.h"

class Goomba
{
private:
	sf::RectangleShape body, topBody, bottomBody;
	sf::Texture texture;
	sf::Vector2f velocity;
	Animator animator;
	float speed;
	int direction;
	float timeLeft;
	bool isColliding;

protected:
	void update_x(float deltaTime, std::vector<Block*> blockVect, std::vector<Goomba*> goombaVect);
	void update_y(float deltaTime, std::vector<Block*> blockVect, std::vector<Goomba*> goombaVect);
	void clump_body();

public:
	bool isActive;
	bool isAlive;
	bool isDead;
	bool isFlat;

	enum ColliderType {
		BODY,
		TOP,
		BOTTOM,
	};

	enum DieWay {
		FALL_OFF,
		FLATTEN,
	};

	Goomba(sf::Texture* texture, Frames& frames);
	~Goomba();
	Collider getCollider(ColliderType type = BOTTOM);
	void draw(sf::RenderWindow& window) const;
	void update(float deltaTime, std::vector<Block*> blockVect, std::vector<Goomba*> goombaVect, Block* bouncingBlock);
	void setPosition(int x, int y, int direction);
	sf::Vector2f getPosition();
	void toggleHitbox();
	void die(DieWay way);
};

