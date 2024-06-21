#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animator.h"
#include "Collider.h"
#include "Block.h"
#include "Goomba.h"
#include "Global.h"

class Player
{
private:
	sf::RectangleShape body;
	sf::Vector2f velocity;
	sf::Texture* texture;
	Animator animator;

	sf::SoundBuffer marioJumpSoundBuffer;
	sf::Sound marioJumpSound;

	sf::SoundBuffer flatSoundBuffer;
	sf::Sound flatSound;

	sf::SoundBuffer marioBlockHitSoundBuffer;
	sf::Sound marioBlockHitSound;

	float speed, jumpHeight, timeLeft;
	bool canJump;

public:
	bool isAlive;
	bool isDead;
	Player(sf::Texture* texture, Frames& frames, float speed, float jumpHeight);
	~Player();
	Collider getCollider();
	sf::Vector2f getPosition();
	sf::Vector2f getSize();
	void setPosition(int x, int y);
	void setVelocity(sf::Vector2f velocity);
	void draw(sf::RenderWindow& window) const;
	void concludeJump(sf::Event action);
	void update(float deltaTime, std::vector<Goomba*> goombaVect, std::vector<Block*> blockVect, Block*& bouncingBlock);
	void die();
	void toggleHitbox();
};

