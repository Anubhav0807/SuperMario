#include "Block.h"


Block::Block(sf::Texture* texture, sf::Vector2f size, bool canBounce) :
    body(sf::Vector2f(16.0f, 16.0f) * SIZE_RATIO)
{
    this->canBounce = canBounce;
    this->texture = texture;

    uvRect.width = size.x;
    uvRect.height = size.y;
    uvRect.left = 17 * uvRect.width;
    uvRect.top = 8 * uvRect.height;
    body.setTexture(texture);
    body.setTextureRect(uvRect);
    body.setOrigin(body.getSize() / 2.0f);
}

Block::~Block() {}

Collider Block::getCollider()
{
    return Collider(body);
}

bool Block::isBouncing() const
{
    return !(velocity.y == 0.0f);
}

void Block::draw(sf::RenderWindow& window) const
{
    window.draw(body);
}

void Block::update(float deltaTime)
{
    velocity.y += 2.0f * GRAVITY * deltaTime;
    body.move(velocity * deltaTime);
    if (body.getPosition().y > initPos.y) {
        body.setPosition(body.getPosition().x, initPos.y);
        velocity.y = 0.0f;
    }
}

void Block::bounce()
{
    if (canBounce) {
        velocity.y = -sqrt(4.0f * GRAVITY * bounceHeight);
    }
}

void Block::setPosition(int x, int y)
{
    initPos = sf::Vector2f(x * 16.0f + 8.0f, y * 16.0f + 8.0f) * SIZE_RATIO;
    body.setPosition(initPos);
}
