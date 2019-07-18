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
	sf::CircleShape			ball;
	sf::RectangleShape		brick;
	sf::RectangleShape		bricks[M][N];

	// Paddle
	float paddleSpeed = 300.0f;
	float paddleHeight = 25.0f;
	float paddleWidth = 200.0f;
	float paddleY = 0.90f;
	
	// Ball
	float radius = 15.0f;
	float ballSpeedX = 150.0f;
	float ballSpeedY = 150.0f;

	// Brick
	float brickHeight = 100.0f;
	float brickWidth = 50.0f;
	float brickGap = 10.0f;
	float windowBuffer = (window.getSize().x - N * brickWidth - (N - 1) * brickGap);


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