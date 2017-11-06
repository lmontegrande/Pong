#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "GameObject.h"

class Ball : public GameObject
{ 
private:
public: 
	sf::CircleShape circleShape; 
	sf::Vector2f direction;
	float speed; 
	float speedModif;

	Ball();
	~Ball();
	void update(float deltaTime);
	void changeDirection(float angle);
	void changeDirection(sf::Vector2f direction);
	void reverseY();
	void reverseX();
	void start();
};

