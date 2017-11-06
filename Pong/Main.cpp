#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp> 
#include "Ball.h"
#include "Paddle.h"
#include "Obstruction.h";
  
using namespace std;
using namespace sf;

struct GameObjectsContainer {
private:
public:
	Ball* _ball;
	Paddle* _leftPaddle;
	Paddle* _rightPaddle;
	Obstruction* _obstruction;
	int lastPaddleToHit = -1;
	bool isCircleCollisionExit;

	GameObjectsContainer(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle, Obstruction* obstruction) {
		_ball = ball;
		_leftPaddle = leftPaddle;
		_rightPaddle = rightPaddle;
		_obstruction = obstruction;
		isCircleCollisionExit = true;
	}
	~GameObjectsContainer() {
		delete _ball;
		delete _leftPaddle;
		delete _rightPaddle;
		delete _obstruction;
	}
};

struct ScoreBoard {
	int _score1;
	int _score2;
	Text* _score1Text;
	Text* _score2Text;

	ScoreBoard(Text* score1Text, Text* score2Text) {
		_score1 = 0;
		_score2 = 0;
		_score1Text = score1Text;
		_score2Text = score2Text;
		_score1Text->setString(to_string(_score1));
		_score2Text->setString(to_string(_score2));
	}

	~ScoreBoard() {
		delete _score1Text;
		delete _score2Text;
	}

	void addPlayer1() {
		_score1++;
		_score1Text->setString(to_string(_score1));
	}

	void addPlayer2() {
		_score2++;
		_score2Text->setString(to_string(_score2));
	}

	void reset() {
		_score1 = 0;
		_score2 = 0;
		_score1Text->setString(to_string(_score1));
		_score2Text->setString(to_string(_score2));
	}
};

void play();
void render(GameObjectsContainer & gameObjects, ScoreBoard &scoreboard, RenderWindow & window);
int handleCollisions(GameObjectsContainer & gameObjects, RenderWindow & window, Sound & sound, Sound & obstructionSound);
void update(GameObjectsContainer&, const float);
bool rectCircleColliding(CircleShape & ball, RectangleShape & paddle);
bool circlesColliding(CircleShape & circle1, CircleShape & circle2);
 
int main()
{
	//TODO: Moving Middle Obstacle, Black Hole
	play();
	return 0;
}

void play() {
	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);
	//MessageBoxA(NULL, path, "Current Directory", MB_OK);

	bool gameIsDone = true;
	Vector2f windowSizeVectorf(800, 600);
	RenderWindow window(VideoMode(windowSizeVectorf.x, windowSizeVectorf.y), "Press Enter To Toggle 2-Players");
	Vector2f windowCenterVectorf(windowSizeVectorf.x / 2, windowSizeVectorf.y / 2);
	Vector2f paddleWindowScaleVectorf = Vector2f(windowSizeVectorf.x / 50, windowSizeVectorf.y / 8);
	Vector2f paddleOffsetFromCenter = Vector2f(windowSizeVectorf.x / 2.5, 0);
	GameObjectsContainer gameObjects(new Ball(), new Paddle(), new Paddle(), new Obstruction());
	ScoreBoard scoreBoard(new Text(), new Text());
	Text endText;
	Clock clock;

	// Load Images
	Texture paddleTexture;
	Texture ballTexture;
	Texture obstructionTexture;
	paddleTexture.loadFromFile("resources/wood.jpg");
	ballTexture.loadFromFile("resources/tennisBall.jpg");
	obstructionTexture.loadFromFile("resources/blackHole.png");

	// Load the sounds used in the game
	SoundBuffer ballSoundBuffer;
	SoundBuffer obstructionSoundBuffer;
	ballSoundBuffer.loadFromFile("resources/ball.wav");
	obstructionSoundBuffer.loadFromFile("resources/jump.wav");
	Sound ballSound(ballSoundBuffer);
	Sound obstructionSound(obstructionSoundBuffer);

	// Initialize the text
	Font font;
	font.loadFromFile("Resources/sansation.ttf");
	scoreBoard._score1Text->setFont(font);
	scoreBoard._score2Text->setFont(font);

	scoreBoard._score1Text->setCharacterSize(40);
	scoreBoard._score2Text->setCharacterSize(40);
	scoreBoard._score1Text->setPosition(windowCenterVectorf.x - 170, 30);
	scoreBoard._score2Text->setPosition(windowCenterVectorf.x + 170, 30);
	scoreBoard._score1Text->setFillColor(Color::White);
	scoreBoard._score2Text->setFillColor(Color::White);

	endText.setFont(font);
	endText.setCharacterSize(50);
	endText.setPosition(windowCenterVectorf);
	endText.setOrigin(Vector2f(windowSizeVectorf.x / 2, 0));
	endText.setFillColor(sf::Color::White);
	endText.setString("Press Space to play");

	//Set up controls
	gameObjects._leftPaddle->controller = 1;
	gameObjects._rightPaddle->controller = 0;

	while (window.isOpen()) {

		if (gameIsDone) {
			// Windows Input Handling
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			window.clear();
			window.draw(*scoreBoard._score1Text);
			window.draw(*scoreBoard._score2Text);
			window.draw(endText);
			window.display();

			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				scoreBoard.reset();
				gameIsDone = false;
				clock.restart();
			}
			continue;
		}

		// Edit values
		bool roundIsDone = false;
		bool isReturnRelease = true;

		// Reset gameObjects
		gameObjects._leftPaddle->rectangleShape.setSize(paddleWindowScaleVectorf);
		gameObjects._leftPaddle->windowHeight = windowSizeVectorf.y;
		gameObjects._leftPaddle->speed = 200;
		gameObjects._leftPaddle->rectangleShape.setOrigin(paddleWindowScaleVectorf / 2.f);
		gameObjects._leftPaddle->rectangleShape.setPosition(windowCenterVectorf - paddleOffsetFromCenter);
		gameObjects._leftPaddle->rectangleShape.setTexture(&paddleTexture, false);

		gameObjects._rightPaddle->rectangleShape.setSize(paddleWindowScaleVectorf);
		gameObjects._rightPaddle->windowHeight = windowSizeVectorf.y;
		gameObjects._rightPaddle->speed = 200;
		gameObjects._rightPaddle->rectangleShape.setOrigin(paddleWindowScaleVectorf / 2.f);
		gameObjects._rightPaddle->rectangleShape.setPosition(windowCenterVectorf + paddleOffsetFromCenter);
		gameObjects._rightPaddle->rectangleShape.setTexture(&paddleTexture, false);

		float ballRadius = 10;
		float randomDirection = std::rand() % 360 + 1;
		gameObjects._ball->direction = Vector2f(cos(randomDirection), sin(randomDirection));
		gameObjects._ball->circleShape.setRadius(ballRadius);
		gameObjects._ball->circleShape.setPosition(windowCenterVectorf);
		gameObjects._ball->speed = 150;
		gameObjects._ball->speedModif = 50;
		gameObjects._ball->circleShape.setOrigin(ballRadius / 2, ballRadius / 2);
		gameObjects._ball->circleShape.setTexture(&ballTexture, false);
		gameObjects._leftPaddle->ballCircleShape = &gameObjects._ball->circleShape;
		gameObjects._rightPaddle->ballCircleShape = &gameObjects._ball->circleShape;

		gameObjects._obstruction->circleShape.setRadius(ballRadius * 8);
		gameObjects._obstruction->circleShape.setPosition(windowCenterVectorf - Vector2f(ballRadius*4, ballRadius*4));
		gameObjects._obstruction->moveSpeed = 75;
		gameObjects._obstruction->circleShape.setOrigin(ballRadius * 4, ballRadius * 4);
		gameObjects._obstruction->circleShape.setTexture(&obstructionTexture, false);
		gameObjects._obstruction->ballCircleShape = &gameObjects._ball->circleShape;

		while (!roundIsDone && window.isOpen()) {

			// Windows Input Handling
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			if (!roundIsDone) {
				// Logic Loops

				if (!Keyboard::isKeyPressed(Keyboard::Return))
					isReturnRelease = true;
				if (Keyboard::isKeyPressed(Keyboard::Return) && isReturnRelease) {
					isReturnRelease = false;
					if (gameObjects._rightPaddle->controller == 2) {
						gameObjects._rightPaddle->controller = 0;
					} else {
						gameObjects._rightPaddle->controller = 2;
					}
				}

				float deltaTime = clock.restart().asSeconds();
				update(gameObjects, deltaTime);
				int results = handleCollisions(gameObjects, window, ballSound, obstructionSound);
				render(gameObjects, scoreBoard, window);

				if (results == 1) {
					scoreBoard.addPlayer1();
					roundIsDone = true;
					gameObjects.lastPaddleToHit = -1;
				}

				if (results == 2) {
					scoreBoard.addPlayer2();
					roundIsDone = true;
					gameObjects.lastPaddleToHit = -1;
				}
				if (scoreBoard._score1 >= 5 || scoreBoard._score2 >= 5) {
					roundIsDone = gameIsDone = true;
					gameObjects.lastPaddleToHit = -1;
				}
			}
		}
	}
}

void render(GameObjectsContainer &gameObjects, ScoreBoard &scoreBoard, RenderWindow &window) {
	window.clear();
	window.draw(gameObjects._obstruction->circleShape);
	window.draw(gameObjects._ball->circleShape);
	window.draw(gameObjects._leftPaddle->rectangleShape);
	window.draw(gameObjects._rightPaddle->rectangleShape);
	window.draw(*scoreBoard._score1Text);
	window.draw(*scoreBoard._score2Text);
	window.display();
}

int handleCollisions(GameObjectsContainer &gameObjects, RenderWindow &window, Sound &ballSound, Sound &obstructionSound) {
	Vector2u windowSize = window.getSize();
	Vector2f ballPosition = gameObjects._ball->circleShape.getPosition();
	float ballRadius = gameObjects._ball->circleShape.getRadius();

	// Hit Left
	if (ballPosition.x <= 0) {
		gameObjects._ball->direction.x = abs(gameObjects._ball->direction.x);
		return 2;
	}
	// Hit Right
	if (ballPosition.x + (ballRadius * 2) >= windowSize.x) {
		gameObjects._ball->direction.x = -abs(gameObjects._ball->direction.x);
		return 1;
	}
	// Hit Top
	if (ballPosition.y - ballRadius <= 0) {
		gameObjects._ball->direction.y = abs(gameObjects._ball->direction.y);
		ballSound.play();
	}
	// Hit Bottom
	if (ballPosition.y + ballRadius >= windowSize.y) {
		gameObjects._ball->direction.y = -abs(gameObjects._ball->direction.y);
		ballSound.play();
	}

	// Ball Hit Obstruction
	if (circlesColliding(gameObjects._ball->circleShape, gameObjects._obstruction->circleShape)) {
		if (gameObjects.isCircleCollisionExit) {
			if (gameObjects.lastPaddleToHit != -1) {
				obstructionSound.play();
				Vector2f deltaVector = gameObjects._obstruction->circleShape.getPosition() - gameObjects._ball->circleShape.getPosition();
				gameObjects._ball->direction = deltaVector / (sqrtf(deltaVector.x * deltaVector.x + deltaVector.y * deltaVector.y));
			}
			gameObjects.lastPaddleToHit = 0;
			gameObjects.isCircleCollisionExit = false;
		}
	} else {
		gameObjects.isCircleCollisionExit = true;
	}

	// Ball Hit left paddle
	if (rectCircleColliding(gameObjects._ball->circleShape, gameObjects._leftPaddle->rectangleShape) && gameObjects.lastPaddleToHit != 1) {
		gameObjects.lastPaddleToHit = 1;
		float deltaY = gameObjects._ball->circleShape.getPosition().y - gameObjects._leftPaddle->rectangleShape.getPosition().y;
		float rectangleHalfHeight = gameObjects._leftPaddle->rectangleShape.getSize().y / 2;
		float diff = Paddle::clampF(deltaY / rectangleHalfHeight, -.9, .9);
		Vector2f newDirection = Vector2f(abs(1 - diff), diff);
		Vector2f newDirectionNormalized = newDirection / sqrt(newDirection.x * newDirection.x + newDirection.y * newDirection.y);
		gameObjects._ball->changeDirection(newDirectionNormalized);
		gameObjects._ball->speed += gameObjects._ball->speedModif;
		ballSound.play();
		/*float angle = diff * 89;
		float sinOfAngle = sin(angle);
		float cosOfAngle = cos(angle);
		Vector2f newDirection = Vector2f(abs(cosOfAngle), sinOfAngle);
		gameObjects._ball->changeDirection(newDirection);
		window.setTitle(to_string(diff) + " " + to_string(angle));*/

	}

	// Ball Hit right paddle
	if (rectCircleColliding(gameObjects._ball->circleShape, gameObjects._rightPaddle->rectangleShape) && gameObjects.lastPaddleToHit != 2) {
		gameObjects.lastPaddleToHit = 2;
		float deltaY = gameObjects._ball->circleShape.getPosition().y - gameObjects._rightPaddle->rectangleShape.getPosition().y;
		float rectangleHalfHeight = gameObjects._rightPaddle->rectangleShape.getSize().y / 2;
		float diff = Paddle::clampF(deltaY / rectangleHalfHeight, -1, 1);
		Vector2f newDirection = Vector2f(-abs(1 - diff), diff);
		Vector2f newDirectionNormalized = newDirection / sqrt(newDirection.x * newDirection.x + newDirection.y * newDirection.y);
		gameObjects._ball->changeDirection(newDirectionNormalized);
		gameObjects._ball->speed += gameObjects._ball->speedModif;
		ballSound.play();
		/*float angle = diff * 89;
		float sinOfAngle = sin(angle);
		float cosOfAngle = cos(angle);
		Vector2f newDirection = Vector2f(-abs(cosOfAngle), sinOfAngle);
		gameObjects._ball->changeDirection(newDirection);
		window.setTitle(to_string(angle) + ": " + to_string(sinOfAngle) + ", " + to_string(sinOfAngle));*/
	}

	return 0;
}

void update(GameObjectsContainer &gameObjects, const float deltaTime) {
	gameObjects._ball->update(deltaTime);
	gameObjects._leftPaddle->update(deltaTime);
	gameObjects._rightPaddle->update(deltaTime);
	gameObjects._obstruction->update(deltaTime);
}  

bool rectCircleColliding(CircleShape &ball,RectangleShape &paddle) {
	float ballRadius = ball.getRadius();
	Vector2f paddleSize = paddle.getSize();

	if (ball.getPosition().x - ballRadius < paddle.getPosition().x + paddleSize.x / 2 &&
		ball.getPosition().x - ballRadius > paddle.getPosition().x &&
		ball.getPosition().y + ballRadius >= paddle.getPosition().y - paddleSize.y / 2 &&
		ball.getPosition().y - ballRadius <= paddle.getPosition().y + paddleSize.y / 2)
		return true;
	if (ball.getPosition().x + ballRadius > paddle.getPosition().x - paddleSize.x / 2 &&
		ball.getPosition().x + ballRadius < paddle.getPosition().x &&
		ball.getPosition().y + ballRadius >= paddle.getPosition().y - paddleSize.y / 2 &&
		ball.getPosition().y - ballRadius <= paddle.getPosition().y + paddleSize.y / 2)
		return true;

	return false;

	/*float distX = abs(circle.getPosition().x - rect.getPosition().x - rect.getSize().x / 2);
	float distY = abs(circle.getPosition().y - rect.getPosition().y - rect.getSize().y / 2);

	if (distX > (rect.getSize().x / 2 + circle.getRadius())) { return false; }
	if (distY > (rect.getSize().y / 2 + circle.getRadius())) { return false; }

	if (distX <= (rect.getSize().x / 2)) { return true; }
	if (distY <= (rect.getSize().y / 2)) { return true; }

	float dx = distX - rect.getSize().x / 2;
	float dy = distY - rect.getSize().y / 2;
	return (dx*dx + dy*dy <= (circle.getRadius()*circle.getRadius()));*/
}

bool circlesColliding(CircleShape &circle1, CircleShape &circle2) {
	float sumRadius = circle1.getRadius() + circle2.getRadius();
	Vector2f deltaVector = circle1.getPosition() - circle2.getPosition();
	float magnitudeDeltaVector = sqrt(deltaVector.x * deltaVector.x + deltaVector.y * deltaVector.y);
	return magnitudeDeltaVector < sumRadius;
}