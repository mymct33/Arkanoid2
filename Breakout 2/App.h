#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

const int M = 12;
const int N = 4;

class App
{
private:
	sf::Event				event;
	sf::View				view;
	sf::RenderWindow		window;
	sf::Time				time;
	sf::Clock				clock;
	sf::RectangleShape		paddle;
	sf::RectangleShape		pLeft;
	sf::RectangleShape		pRight;
	sf::CircleShape			ball;
	sf::RectangleShape		brick;
	sf::RectangleShape		bricks[M][N];

	// Paddle
	float paddleSpeed = 300.0f;
	float paddleHeight = 25.0f;
	float paddleWidth = 66.6f;
	float paddleY = 0.90f;
	
	// Ball
	float radius = 15.0f;
	float ballSpeed = 150.0f;
	float ballSpeedX;
	float ballSpeedY;

	// Brick
	float brickWidth = 100.0f;
	float brickHeight = 25.0f;
	float brickGap = 2.5f;
	float windowBuffer;
	bool collidable[M][N];


public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);

	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();
	void Collisions();
};