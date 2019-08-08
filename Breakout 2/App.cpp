#include <iostream>
#include "SFML\System.hpp"
#include "App.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

App::App(const char* title, int screenWidth, int screenHeight, int screenBpp)
{
	// Setting up the game window
	window.create(sf::VideoMode(screenWidth, screenHeight, screenBpp), title);
	window.setFramerateLimit(0);
	view = window.getDefaultView();
	window.resetGLStates();
}

App::~App()
{
	for (int i = 0; i < ROW; ++i)
	{
		delete[] bricks[i];
		delete[] collidable[i];
	}
	delete[] bricks;
	delete[] collidable;
}

bool App::Init()
{
	windowBuffer = (window.getSize().x - COL * brickWidth - (COL - 1) * brickGap)/2;
	ballSpeedX = ballSpeed;
	ballSpeedY = ballSpeed;

	ImGui::SFML::Init(window);
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
	paddle.setPosition(sf::Vector2f((window.getSize().x / 2.0f) - (paddleWidth / 2.0f), window.getSize().y * paddleY));
	pLeft.setPosition(sf::Vector2f(((window.getSize().x / 2.0f) - paddleWidth) - (paddleWidth / 2.0f), window.getSize().y * paddleY));
	pRight.setPosition(sf::Vector2f(((window.getSize().x / 2.0f) + paddleWidth) - (paddleWidth / 2.0f), window.getSize().y * paddleY));

	// Giving bricks initial position
	bricks = new sf::RectangleShape * [ROW];
	collidable = new bool* [ROW];
	for (int i = 0; i < ROW; ++i)
	{
		bricks[i] = new sf::RectangleShape[COL];
		collidable[i] = new bool[COL];
	}
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			bricks[i][j].setSize(sf::Vector2f(brickWidth, brickHeight));
			bricks[i][j].setPosition(windowBuffer + (brickWidth + brickGap) * j, (brickHeight + brickGap) * i);
			collidable[i][j] = true;
		}
	}

	return true;
}

void App::Update()
{
	ImGui::SFML::Update(window, clock.restart());

	// starting clock
	deltaTime = clock.restart().asSeconds();

	// scene control
	// main menu
	if (scene == 0)
	{
		ImGui::Begin("Menu");
		if (ImGui::Button("Play"))
		{
			scene = 1;
		}
		if (ImGui::Button("Editor"))
		{
			scene = 2;
		}
		ImGui::End();
	}
	
	// main game
	if (scene == 1)
	{
		PlayerMove();

		bFreeze = 1.0f;

		Collisions();
		
	}

	//editor
	if (scene == 2)
	{
		bFreeze = 0.0f;
		Editor();
	}

	// Ball moves per frame
	ball.move((ballSpeedX * deltaTime) * bFreeze, (ballSpeedY * deltaTime) * bFreeze);
}

void App::Draw()
{
	// Drawing everything into game window
	window.clear();
	window.setView(view);

	ImGui::SFML::Render(window);

	if(scene == 1)
	{
		window.draw(ball);
		window.draw(paddle);
		window.draw(pLeft);
		window.draw(pRight);

		for (int i = 0; i < ROW; ++i)
		{
			for (int j = 0; j < COL; ++j)
			{
				if (collidable[i][j])
				{
					window.draw(bricks[i][j]);
				}
			}
		}
	}

	if (scene == 2)
	{
		for (int i = 0; i < ROW; ++i)
		{
			for (int j = 0; j < COL; ++j)
			{
				if (collidable[i][j])
				{
					window.draw(bricks[i][j]);
				}
			}
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
			ImGui::SFML::ProcessEvent(event);
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
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			if (ball.getGlobalBounds().intersects(bricks[i][j].getGlobalBounds()) && collidable[i][j])
			{
				ballSpeedY = -ballSpeedY;
				collidable[i][j] = false;
				++score;
				std::cout << score + "\n";
			}
		}
	}
}

void App::PlayerMove()
{
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

}

void App::Editor()
{
	for (int i = 0; i < ROW; ++i)
	{
		for (int j = 0; j < COL; ++j)
		{
			if (bricks[i][j].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						collidable[i][j] = !collidable[i][j];
					}
				}
			}
		}
	}
}