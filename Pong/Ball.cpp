#include "Ball.h"   

Ball::Ball()
{  
}
 
Ball::~Ball()
{
}

void Ball::start()
{
}

void Ball::update(float deltaTime)
{
	circleShape.move(direction * speed * deltaTime);
	circleShape.setOrigin(circleShape.getRadius(), circleShape.getRadius());
	circleShape.rotate(speed * deltaTime);
}

void Ball::changeDirection(float angle) {
	direction = sf::Vector2f(cos(angle), sin(angle));
}

void Ball::changeDirection(sf::Vector2f dir) {
	direction = sf::Vector2f(dir); // Assumes dir has been normalized
}

void Ball::reverseY() {
	direction.y = -direction.y;
}

void Ball::reverseX() {
	direction.x = -direction.x;
}