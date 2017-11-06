#include "Paddle.h"
 
Paddle::Paddle()
{
}
 
Paddle::~Paddle()
{
}

void Paddle::update(float deltaTime)
{
	sf::Keyboard::Key upKey;
	sf::Keyboard::Key downKey;
	switch (controller) {
	case 1:
		upKey = sf::Keyboard::W;
		downKey = sf::Keyboard::S;
		break;
	case 2:
		upKey = sf::Keyboard::Up;
		downKey = sf::Keyboard::Down;
		break;
	default:
		ai(deltaTime);
		return;
	}

	if (sf::Keyboard::isKeyPressed(upKey) && rectangleShape.getPosition().y - rectangleShape.getSize().y / 2 >= 0)
		rectangleShape.move(sf::Vector2f(0, -1) * speed * deltaTime); 

	if (sf::Keyboard::isKeyPressed(downKey) && rectangleShape.getPosition().y + rectangleShape.getSize().y / 2 <= windowHeight) 
		rectangleShape.move(sf::Vector2f(0, 1) * speed * deltaTime); 
}

void Paddle::SetPositionFromCenter(sf::Vector2f targetPosition) {
	rectangleShape.move(targetPosition - sf::Vector2f(rectangleShape.getSize().x/2, rectangleShape.getSize().y / 2));
}

void Paddle::start()
{

}

void Paddle::ai(float deltaTime) {
	float paddleSizeY = rectangleShape.getSize().y;
	float ballY = (*ballCircleShape).getPosition().y;
	float deltaY = ballY - rectangleShape.getPosition().y;
	float targetY = clampF(ballY, paddleSizeY/2, windowHeight - paddleSizeY/2);
	float clampedTargetY = clampF(targetY, rectangleShape.getPosition().y - speed*deltaTime, rectangleShape.getPosition().y + speed*deltaTime);
	sf::Vector2f deltaPos = sf::Vector2f(rectangleShape.getPosition().x, clampedTargetY);
	rectangleShape.move(deltaPos - rectangleShape.getPosition());
}

float Paddle::clampF(float value, float min, float max) {
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}
