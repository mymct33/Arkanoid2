#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"


class App
{
private:
	sf::Event				event;
	sf::View				view;
	sf::RenderWindow		window;
	sf::Time				time;
	sf::Clock				clock;
	sf::Vector2i			mousePos;
	sf::RectangleShape		paddle;
	sf::RectangleShape		pLeft;
	sf::RectangleShape		pRight;
	sf::CircleShape			ball;
	sf::RectangleShape**	bricks;

	int scene = 0; // 0 = menu, 1 = game, 2 = editor
	const int ROW = 4;
	const int COL = 12;
	float deltaTime;

	// Buttons
	float playHeight = 50.0f;
	float playWidth = 100.0f;
	float playX = 590.0f;
	float playY = 100.0f;

	// Paddle
	float paddleSpeed = 3000.0f;
	float paddleHeight = 25.0f;
	float paddleWidth = 66.6f;
	float paddleY = 0.90f;
	
	// Ball
	float radius = 15.0f;
	float ballSpeed = 1500.0f;
	float ballSpeedX;
	float ballSpeedY;
	float bFreeze = 0;

	// Brick
	float brickWidth = 100.0f;
	float brickHeight = 50.0f;
	float brickGap = 2.5f;
	float windowBuffer;
	bool** collidable;

	int score;

public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);

	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();
	void Collisions();
	void PlayerMove();
	void Editor();
};