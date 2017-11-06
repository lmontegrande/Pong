#include "Obstruction.h"



Obstruction::Obstruction()
{
}


Obstruction::~Obstruction()
{
}

void Obstruction::update(float deltaTime)
{
	circleShape.rotate(moveSpeed * deltaTime);
	sf::Vector2f deltaVector = (*ballCircleShape).getPosition() - circleShape.getPosition();
	sf::Vector2f deltaVectorNormalized = deltaVector / sqrt(deltaVector.x*deltaVector.x + deltaVector.y*deltaVector.y);
	circleShape.move(deltaVectorNormalized * moveSpeed * deltaTime);
	circleShape.setOrigin(circleShape.getRadius(), circleShape.getRadius());
}

void Obstruction::start()
{
}
