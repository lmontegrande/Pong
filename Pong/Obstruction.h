#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "GameObject.h"

class Obstruction : public GameObject
{
public:
	sf::CircleShape circleShape; // Includes pullRadius and position
	sf::Vector2f direction;
	sf::CircleShape *ballCircleShape;
	float pullStrength;
	float moveSpeed;

	Obstruction();
	~Obstruction();

	// Inherited via GameObject
	virtual void update(float deltaTime) override;
	virtual void start() override;
};

