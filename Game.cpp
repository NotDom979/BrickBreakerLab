#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}
bool Lose = false;
bool Win = false;
void Game::Reset()
{
	Bricks.clear();

	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	brick.width = 12;
	brick.height = 2;
	brick.x_position = 0;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;
	Bricks.push_back(brick);
	for (int i = 0; i < 4; i++)
	{
		brick.x_position += 15;
		Bricks.push_back(brick);
	}
	Lose = false;
	Win = false;
}

void Game::ResetBall() 
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering should be done between the locks in this function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < Bricks.size(); i++)
	{
		Bricks[i].Draw();
	}

	if (Lose == true)
	{
		char const t[32] = "You Lose, Press R to restart";
		Console::WordWrap(20, 15, 35, t);
	}
	if (Win == true)
	{
		char const w[32] = "You Win! Press R to restart";
		Console::WordWrap(20, 15, 35, w);
	}
	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < Bricks.size(); i++)
	{
		if (Bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			Bricks[i].color = ConsoleColor(Bricks[i].color - 1);
			ball.y_velocity *= -1;

		// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (Bricks[i].color == Black)
			{
				Bricks.erase(Bricks.begin() + i);
				Bricks.shrink_to_fit();
			}
		}

	}

	// TODO #6 - If no bricks remain, pause ball and display victory text with R to reset
	if (Bricks.size() == 0)
	{
		ball.x_velocity = 0;
		ball.y_velocity = 0;
		Win = true;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display defeat text with R to reset
	if (ball.y_position == 35)
	{
		ball.x_velocity = 0;
		ball.y_velocity = 0;
		Lose = true;
	}
}
