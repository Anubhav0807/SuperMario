#include "Goomba.h"

Goomba::Goomba(sf::Texture* texture, Frames& frames) :
    body(sf::Vector2f(15.9f, 15.9f) * SIZE_RATIO),
    topBody(sf::Vector2f(12.0f, 5.9f) * SIZE_RATIO),
    bottomBody(sf::Vector2f(15.9f, 10.0f) * SIZE_RATIO),
    animator(&body, texture, frames, sf::Vector2u(6, 4), 0.3f)
{
    direction = 0;
    speed = 100.0f;
    timeLeft = 0.0f;
    isActive = false;
    isAlive = true;
    isDead = false;
    isFlat = false;
    isColliding = false;

    body.setTexture(texture);
    body.setOrigin(body.getSize() / 2.0f);

    topBody.setFillColor(sf::Color::Transparent);
    topBody.setOutlineColor(sf::Color::Green);
    topBody.setOrigin(topBody.getSize() / 2.0f);

    bottomBody.setFillColor(sf::Color::Transparent);
    bottomBody.setOutlineColor(sf::Color::Red);
    bottomBody.setOrigin(bottomBody.getSize() / 2.0f);
}

Goomba::~Goomba() {}

Collider Goomba::getCollider(ColliderType type)
{
    switch (type) {
    case BODY:
        return Collider(body);
    case TOP:
        return Collider(topBody);
    case BOTTOM:
        return Collider(bottomBody);
    default:
        return Collider(body);
    }
}

void Goomba::draw(sf::RenderWindow& window) const
{
    if (not isDead) {
        window.draw(body);
        if (topBody.getOutlineThickness() != 0.0f)
            window.draw(topBody);
        if (topBody.getOutlineThickness() != 0.0f)
            window.draw(bottomBody);
    }
}

void Goomba::update(float deltaTime, std::vector<Block*> blockVect, std::vector<Goomba*> goombaVect, Block* bouncingBlock)
{
    if (isFlat && not isDead) {
        timeLeft -= deltaTime;
        if (timeLeft < 0.0f) {
            isDead = true;
        }
        animator.setState(Animator::DYING);
        animator.update(deltaTime, 1.0f);
        return;
    }
    if (isAlive && bouncingBlock) {
        if (bouncingBlock->getCollider().checkCollision(getCollider())) {
            die(FALL_OFF);
        }
    }
    if (isActive)
    {
        velocity.x = speed * direction;
        velocity.y += GRAVITY * deltaTime;
        update_x(deltaTime, blockVect, goombaVect);
        update_y(deltaTime, blockVect, goombaVect);
        clump_body();
    }
    if (isAlive) {
        animator.setState(Animator::WALKING);
        animator.update(deltaTime, 1.0f);
    }
    else {
        if (not isDead && body.getPosition().y > SCREEN_HEIGHT) {
            isDead = true;
            animator.flip_y();
        }
    }
}

void Goomba::update_x(float deltaTime, std::vector<Block*> blockVect, std::vector<Goomba*> goombaVect)
{
    bottomBody.move(sf::Vector2f(velocity.x, 0.0f) * deltaTime);

    if (isAlive) {
        // Check horizontal collision
        for (Block* block : blockVect) {
            if (block->getCollider().checkCollision(getCollider())) {
                bottomBody.move(sf::Vector2f(-velocity.x, 0.0f) * deltaTime);
                direction *= -1;
                return;
            }
        }
        for (Goomba* goomba : goombaVect) {
            if (goomba == this || not goomba->isAlive) continue;
            if (goomba->getCollider().checkCollision(getCollider())) {
                if (getPosition().x < goomba->getPosition().x && velocity.x > 0.0f || getPosition().x > goomba->getPosition().x && velocity.x < 0.0f) {
                    bottomBody.move(sf::Vector2f(-velocity.x, 0.0f) * deltaTime);
                }
                direction *= -1;
                return;
            }
        }
    }
}

void Goomba::update_y(float deltaTime, std::vector<Block*> blockVect, std::vector<Goomba*> goombaVect)
{
    bottomBody.move(sf::Vector2f(0.0f, velocity.y) * deltaTime);

    if (isAlive) {
        // Check vertical collision
        for (Block* block : blockVect) {
            if (block->getCollider().checkCollision(getCollider())) {
                bottomBody.move(sf::Vector2f(0.0f, -velocity.y) * deltaTime);
                velocity.y = 0.0f;
                return;
            }
        }
        for (Goomba* goomba : goombaVect) {
            if (goomba == this || not goomba->isAlive) continue;
            if (goomba->getCollider(TOP).checkCollision(getCollider())) {
                bottomBody.move(sf::Vector2f(0.0f, -velocity.y) * deltaTime);
                velocity.y = 0.0f;
                if (not isColliding) {
                    isColliding = true;
                    if (getPosition().x < goomba->getPosition().x) {
                        direction = -1;
                        goomba->direction = +1;
                    }
                    else {
                        direction = +1;
                        goomba->direction = -1;
                    }
                }
                return;
            }
            else {
                isColliding = false;
            }
        }
    }
}

void Goomba::clump_body()
{
    topBody.setPosition(bottomBody.getPosition() + sf::Vector2f(0.0f, -7.95f) * SIZE_RATIO);
    body.setPosition(bottomBody.getPosition() + sf::Vector2f(0.0f, -10.0f));
}

void Goomba::setPosition(int x, int y, int direciton)
{
    bottomBody.setPosition(sf::Vector2f(x * 16.0f + 8.0f, y * 16.0f + 8.0f) * SIZE_RATIO);
    clump_body();
    this->direction = std::min(std::max(direciton, -1), +1); // Clamping
    isActive = false;
    isAlive = true;
    isDead = false;
    isFlat = false;
    isColliding = false;
}

sf::Vector2f Goomba::getPosition()
{
    return body.getPosition();
}

void Goomba::toggleHitbox()
{
    if (topBody.getOutlineThickness() == 0.0f) {
        topBody.setOutlineThickness(2.0f);
        bottomBody.setOutlineThickness(2.0f);
    }
    else {
        topBody.setOutlineThickness(0.0f);
        bottomBody.setOutlineThickness(0.0f);
    }
}

void Goomba::die(DieWay way)
{
    switch (way) {
    case FALL_OFF:
        isAlive = false;
        velocity.y = -200.0f;
        animator.flip_y();
        break;
    case FLATTEN:
        isAlive = false;
        isFlat = true;
        timeLeft = 1.5f;
        break;
    }
}