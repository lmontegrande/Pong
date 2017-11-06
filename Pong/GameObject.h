#pragma once
class GameObject
{
public:
	virtual void update(float deltaTime) = 0;
	virtual void start() = 0;
};

