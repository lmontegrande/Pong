#pragma once  
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "GameObject.h"

class Paddle : public GameObject
{
private: 
public:
	sf::RectangleShape rectangleShape;
	sf::Vector2f direction;
	sf::CircleShape *ballCircleShape;
	float speed; 
	float windowHeight;
	int controller;

	Paddle();
	~Paddle(); 
	void update(float deltaTime);
	void start();
	void SetPositionFromCenter(sf::Vector2f targetPosition);
	static float clampF(float value, float min, float max);
	void ai(float deltaTime);
};
