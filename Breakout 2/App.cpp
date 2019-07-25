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
	windowBuffer = (window.getSize().x - M * brickWidth - (M - 1) * brickGap)/2;
	ballSpeedX = ballSpeed;
	ballSpeedY = ballSpeed;

	// Giving the ball their initial positions
	ball.setRadius(radius);
	ball.setPosition(sf::Vector2f(0.5f * window.getSize().x, 0.8f * window.getSize().y));

	// Paddle size and position. Moves end pieces relative to main paddle
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	paddle.setFillColor(sf::Color::Green);
	pLeft.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	pLeft.setFillColor(sf::Color::Red);
	pRight.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	pRight.setFillColor(sf::Color::Blue);
	paddle.setPosition(sf::Vector2f((window.getSize().x / 2.0f) - (paddleWidth / 2.0f), window.getSize().y  * paddleY));
	pLeft.setPosition(sf::Vector2f(((window.getSize().x / 2.0f) - paddleWidth) - (paddleWidth / 2.0f), window.getSize().y * paddleY));
	pRight.setPosition(sf::Vector2f(((window.getSize().x / 2.0f) + paddleWidth) - (paddleWidth / 2.0f), window.getSize().y * paddleY));

	// Setting Brick to correct size
	brick.setSize(sf::Vector2f(brickWidth, brickHeight));

	// Giving bricks initial position
	for (int j = 0; j < M; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			bricks[j][i].setSize(sf::Vector2f(brickWidth, brickHeight));
			bricks[j][i].setPosition(windowBuffer + (brickWidth + brickGap) * j, (brickHeight + brickGap) * i);
			collidable[j][i] = true;
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
		pLeft.move(paddleSpeed * deltaTime, 0.0f);
		pRight.move(paddleSpeed * deltaTime, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		paddle.move(-paddleSpeed * deltaTime, 0.0f);
		pLeft.move(-paddleSpeed * deltaTime, 0.0f);
		pRight.move(-paddleSpeed * deltaTime, 0.0f);
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
	window.draw(pLeft);
	window.draw(pRight);
	for (int j = 0; j < M; ++j)
	{
		for (int i = 0; i < N; i++)
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
	// Ball on Wall collision
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
		//window.close();
	}

	// Paddle on Wall collision
	if (pLeft.getPosition().x < 0)
	{
		pLeft.setPosition(0.0f, window.getSize().y * paddleY);
	}
	if (pRight.getPosition().x > window.getSize().x - paddleWidth)
	{
		pRight.setPosition(window.getSize().x - paddleWidth, window.getSize().y * paddleY);
	}

	// Paddle on Paddle collision
	if (paddle.getGlobalBounds().intersects(pLeft.getGlobalBounds()))
	{
		paddle.setPosition(pLeft.getPosition().x + paddleWidth, window.getSize().y * paddleY);
		pRight.setPosition(pLeft.getPosition().x + (2.0f * paddleWidth), window.getSize().y * paddleY);
	}
	if (paddle.getGlobalBounds().intersects(pRight.getGlobalBounds()))
	{
		paddle.setPosition(pRight.getPosition().x - paddleWidth, window.getSize().y * paddleY);
		pLeft.setPosition(pRight.getPosition().x - (2.0f * paddleWidth), window.getSize().y * paddleY);
	}

	// Ball on Paddle collision
	if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()))
	{
		ballSpeedY = -ballSpeedY;
	}
	if (ball.getGlobalBounds().intersects(pLeft.getGlobalBounds()))
	{
		ballSpeedX = -ballSpeed;
		ballSpeedY = -ballSpeed;
	}
	if (ball.getGlobalBounds().intersects(pRight.getGlobalBounds()))
	{
		ballSpeedX = ballSpeed;
		ballSpeedY = -ballSpeed;
	}

	// Ball on Brick collision
	for (int j = 0; j < M; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			if (ball.getGlobalBounds().intersects(bricks[j][i].getGlobalBounds()) && collidable[j][i])
			{
				ballSpeedY = -ballSpeedY;
				collidable[j][i] = false;
			}

			if (collidable[j][i] == false)
			{
				bricks[j][i].setFillColor(sf::Color::Transparent);
			}
		}
	}
}