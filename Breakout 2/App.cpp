#include "SFML\System.hpp"
#include "App.h"

App::App(const char* title, int screenWidth, int screenHeight, int screenBpp)
{
	// Setting up the game window
	window.create(sf::VideoMode(screenWidth, screenHeight, screenBpp), title);
	window.setFramerateLimit(60);
	view = window.getDefaultView();
}

App::~App()
{

}

bool App::Init()
{
	// Giving the ball/paddle their initial positions
	ball.setRadius(radius);
	ball.setPosition(sf::Vector2f(0.5f * window.getSize().x, 0.8f * window.getSize().y));
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	paddle.setPosition(sf::Vector2f((window.getSize().x / 2.0f) - (paddleWidth / 2.0f), window.getSize().y  * paddleY));

	// Setting Brick to correct size
	brick.setSize(sf::Vector2f(brickWidth, brickHeight));

	// Giving bricks initial position
	for (int j = 0; j < M; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			bricks[j][i].setSize(sf::Vector2f(brickWidth, brickHeight));
			bricks[j][i].setPosition(windowBuffer + (brickWidth + brickGap) * i, (brickHeight + brickGap) * j);
		}
	}

	return true;
}

void App::Update()
{
	// starting clock
	float deltaTime = clock.restart().asSeconds();

	// Reading player input to move paddle
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		paddle.move(paddleSpeed * deltaTime, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		paddle.move(-paddleSpeed * deltaTime, 0.0f);
	}

	// Ball moves per frame
	ball.move(ballSpeedX * deltaTime, ballSpeedY * deltaTime);

	Collisions();
}

void App::Draw()
{
	// Drawing everything into game window
	window.clear();
	window.setView(view);
	window.draw(ball);
	window.draw(paddle);

	for (int j = 0; j < M; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			window.draw(bricks[j][i]);
		}
	}

	window.display();
}

void App::HandleEvents()
{
	// Closing the game, closes the game
	if (event.type == sf::Event::Closed)
	{
		window.close();
	}
}

void App::Run()
{
	// Basic game loop
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			HandleEvents();
		}
		Update();
		Draw();
	}
}

// All collision detection
void App::Collisions()
{
	if (ball.getPosition().x < 0)
	{
		ballSpeedX = -ballSpeedX;
	}

	if (ball.getPosition().x > window.getSize().x - radius * 2.0f)
	{
		ballSpeedX = -ballSpeedX;
	}

	if (ball.getPosition().y < 0)
	{
		ballSpeedY = -ballSpeedY;
	}

	if (ball.getPosition().y > window.getSize().y - radius * 2.0f)
	{
		ballSpeedY = -ballSpeedY;
	}

	if (paddle.getPosition().x < 0)
	{
		paddle.setPosition(0.0f, window.getSize().y * paddleY);
	}

	if (paddle.getPosition().x > window.getSize().x - paddleWidth)
	{
		paddle.setPosition(window.getSize().x - paddleWidth, window.getSize().y * paddleY);
	}

	if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()))
	{
		ballSpeedY = -ballSpeedY;
	}
}