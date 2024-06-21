#include "Player.h"

Player::Player(sf::Texture* texture, Frames& frames, float speed, float jumpHeight) :
	body(sf::Vector2f(15.9f, 15.9f) * SIZE_RATIO),
	texture(texture),
	animator(&body, texture, frames, sf::Vector2u(7, 4), 0.07f)
{
	this->speed = speed * SIZE_RATIO;
	this->jumpHeight = jumpHeight * SIZE_RATIO;
	timeLeft = 0.0f;
	canJump = true;
	isAlive = true;
	isDead = false;

	marioJumpSoundBuffer.loadFromFile(PATH.jump_sound);
	marioJumpSound.setBuffer(marioJumpSoundBuffer);
	marioJumpSound.setVolume(5.0f);

	flatSoundBuffer.loadFromFile(PATH.flat_sound);
	flatSound.setBuffer(flatSoundBuffer);
	flatSound.setVolume(100.0f);

	marioBlockHitSoundBuffer.loadFromFile(PATH.mario_block_hit_sound);
	marioBlockHitSound.setBuffer(marioBlockHitSoundBuffer);
	marioBlockHitSound.setVolume(100.0f);

	body.setOrigin(body.getSize() / 2.0f);
	body.setTexture(texture);
}

Player::~Player() {

}

sf::Vector2f Player::getPosition()
{
	return body.getPosition();
}

sf::Vector2f Player::getSize()
{
	return body.getSize();
}

void Player::setPosition(int x, int y)
{
	body.setPosition(sf::Vector2f(x * 16.0f + 8.0f, y * 16.0f + 8.0f) * SIZE_RATIO);
	animator.setDirection(Animator::RIGHT);
}

void Player::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}

Collider Player::getCollider()
{
	return Collider(body);
}

void Player::draw(sf::RenderWindow& window) const {
	window.draw(body);
}

void Player::concludeJump(sf::Event action)
{
	if (action.type == sf::Event::KeyReleased && isAlive &&
		(action.key.code == sf::Keyboard::Space || action.key.code == sf::Keyboard::Up || action.key.code == sf::Keyboard::W))
	{
		if (velocity.y < -sqrt(2.0f * GRAVITY * jumpHeight * 0.15f)) {
			velocity.y = -sqrt(2.0f * GRAVITY * jumpHeight * 0.15f);
		}
	}
}

void Player::update(float deltaTime, std::vector<Goomba*> goombaVect, std::vector<Block*> blockVect, Block*& bouncingBlock) {

	if (isAlive)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (velocity.x > 0) {
				velocity.x -= 2.0f * FRICTION * deltaTime;
			}
			else {
				velocity.x -= FRICTION * deltaTime;
			}
			if (velocity.x < -speed) {
				velocity.x = -speed;
			}
			animator.setDirection(Animator::LEFT);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (velocity.x < 0) {
				velocity.x += 2.0f * FRICTION * deltaTime;
			}
			else {
				velocity.x += FRICTION * deltaTime;
			}
			if (velocity.x > speed) {
				velocity.x = speed;
			}
			animator.setDirection(Animator::RIGHT);
		}
		else
		{
			if (velocity.x > 0) {
				velocity.x -= FRICTION * deltaTime;
				if (velocity.x < 0) {
					velocity.x = 0.0f;
				}
			}
			else if (velocity.x < 0) {
				velocity.x += FRICTION * deltaTime;
				if (velocity.x > 0) {
					velocity.x = 0.0f;
				}
			}
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::W)) &&
			canJump)
		{
			if (velocity.y == 0.0f)
			{
				canJump = false;

				// v^2 = u^2 + 2as
				// u^2 = v^2 - 2as
				// u^2 = 2as (here, v = 0)
				// u = sqrt(2*a*s)
				velocity.y = -sqrt(2.0f * GRAVITY * jumpHeight);
				// https://youtu.be/v1V3T5BPd7E?si=fteIctFsT8JLRlXz
				marioJumpSound.play();
			}
		}

		velocity.y += GRAVITY * deltaTime;

		/*----------------------------------------COLLISION DETECTION----------------------------------------*/
		body.move(sf::Vector2f(velocity.x, 0.0f) * deltaTime);
		// Check horizontal collision
		for (Block* block : blockVect) {
			if (block->getCollider().checkCollision(getCollider())) {
				body.move(sf::Vector2f(-velocity.x, 0.0f) * deltaTime);
				velocity.x = 0;
				break;
			}
		}

		body.move(sf::Vector2f(0.0f, velocity.y) * deltaTime);
		// Check vertical collision
		for (Block* block : blockVect) {
			if (block->getCollider().checkCollision(getCollider())) {
				body.move(sf::Vector2f(0.0f, -velocity.y) * deltaTime);
				if (velocity.y > 0.0f) {
					canJump = true;
				}
				else if (velocity.y < 0.0f) {
					marioBlockHitSound.play();
					if (bouncingBlock == nullptr) {
						block->bounce();
						bouncingBlock = block;
					}
				}
				velocity.y = 0;
				break;
			}
		}

		for (Goomba* goomba : goombaVect) {
			if (goomba->isAlive && goomba->getCollider(Goomba::TOP).checkCollision(getCollider())) {
				goomba->die(Goomba::FLATTEN);
				velocity.y = -sqrt(2.0f * GRAVITY * 10.0f * SIZE_RATIO); // Bounce feedback
				flatSound.play();
			}
			else if (goomba->isAlive && goomba->getCollider().checkCollision(getCollider())) {
				die();
			}
		}
		/*-------------------------------------END OF COLLISION DETECTION-------------------------------------*/

		// Managing Animation
		if (abs(velocity.y) > 100.0f || (velocity.y != 0.0f && canJump == false)) {
			animator.setState(Animator::JUMPING);
		}
		else if (velocity.x != 0.0f) {
			if (velocity.x > 0.0f &&
				(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ||
				velocity.x < 0.0f &&
				(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::D))) {
				animator.setState(Animator::DRAGGING);
			}
			else {
				animator.setState(Animator::WALKING);
			}
		}
		else {
			animator.setState(Animator::IDLE);
		}
	}
	else if (not isDead) {
		if (timeLeft < 0.0f) {
			velocity.y += GRAVITY * deltaTime;
			body.move(velocity * deltaTime);
			if (body.getPosition().y > SCREEN_HEIGHT + body.getSize().y / 2.0f) {
				isDead = true;
			}
		}
		else {
			timeLeft -= deltaTime;
		}
		animator.setState(Animator::DYING);
	}
	animator.update(deltaTime, abs(velocity.x) / speed);
}

void Player::die()
{
	isAlive = false;
	animator.setState(Animator::DYING);
	velocity.x = 0.0f;
	velocity.y = -sqrt(2.0f * GRAVITY * 40.0f * SIZE_RATIO);
	timeLeft = 0.5f;
}

void Player::toggleHitbox()
{
	if (body.getOutlineThickness() == 0) {
		body.setOutlineThickness(2);
	}
	else {
		body.setOutlineThickness(0);
	}
}
