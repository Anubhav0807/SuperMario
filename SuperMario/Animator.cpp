#include "Animator.h"

Animator::Animator(sf::RectangleShape* body, sf::Texture* texture, Frames& frames, sf::Vector2u imageCount, float switchTime) :
	body(body),
	imageCount(imageCount),
	curImage(0, 0),
	frames(frames)
{
	totalTime = 0.0f;
	curState = IDLE;
	curDirection = RIGHT;
	curFrame = 0;
	this->switchTime = switchTime;
	uvRect.width  = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);
}

Animator::~Animator() {}

void Animator::update(float deltaTime, float speedPercent)
{
	totalTime += deltaTime * speedPercent;
	if (totalTime >= switchTime) {
		totalTime -= switchTime;
		curFrame++;
		if (curFrame > 99999) {
			curFrame = 0;
		}
	}
	setState(curState);
	body->setTextureRect(uvRect);
}

void Animator::setState(State state)
{
	curState = state;
	switch (state) {
	case IDLE:
		curImage = frames.idle[curFrame % frames.idle.size()];
		break;
	case WALKING:
		curImage = frames.walking[curFrame % frames.walking.size()];
		break;
	case JUMPING:
		curImage = frames.jumping[curFrame % frames.jumping.size()];
		break;
	case DRAGGING:
		curImage = frames.dragging[curFrame % frames.dragging.size()];
		break;
	case DYING:
		curImage = frames.dying[curFrame % frames.dying.size()];
		break;
	}

	if (curDirection == LEFT) {
		uvRect.left = abs(uvRect.width) * curImage.x;
		uvRect.left = uvRect.left + abs(uvRect.width);
	}
	else if (curDirection == RIGHT) {
		uvRect.left = abs(uvRect.width) * curImage.x;
	}
	uvRect.top = uvRect.height * curImage.y;
}

void Animator::setDirection(Direction direction)
{
	if (curDirection == LEFT && direction == RIGHT) {
		uvRect.width = +abs(uvRect.width);
		uvRect.left = uvRect.left - abs(uvRect.width) - 1;
	}
	else if (curDirection == RIGHT && direction == LEFT) {
		uvRect.width = -abs(uvRect.width);
		uvRect.left = uvRect.left + abs(uvRect.width) + 1;
	}
	// else do nothing
	curDirection = direction;
}

Animator::State Animator::getState() const
{
	return curState;
}

Animator::Direction Animator::getDirection() const
{
	return curDirection;
}

void Animator::flip_x() {

}

void Animator::flip_y() {
	uvRect.height *= -1;
	uvRect.top -= uvRect.height;
	body->setTextureRect(uvRect);
}