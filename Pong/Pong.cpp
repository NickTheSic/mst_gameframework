#include "Pong.h"

#include <iostream>
#define dbgval(val) std::cout << #val << ": " << val << std::endl;
#define dbglog(msg) std::cout << msg << std::endl;

std::ostream& operator<<(std::ostream& os, const v2i& v)
{
	os << "X: " << v.x << " Y: " << v.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const v2f& v)
{
	os << "X: " << v.x << " Y: " << v.y;
	return os;
}

mst::Engine* mst::Engine::CreateEngine()
{
	return new MyGame(300, 400);
}

MyGame::~MyGame()
{
	if (QuadRenderer)
	{
		delete QuadRenderer;
	}

	if (TextRenderer)
	{
		delete TextRenderer;
	}
}

bool MyGame::UserStartup()
{
	glClearDepth(1.0f);

	glClearColor(0.1f,0.1f,0.1f,1.f);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MainCamera.Position = ScreenSize / 2;

	QuadRenderer = new mst::QuadRenderer(10);
	QuadRenderer->UseProgram();
	QuadRenderer->SetUniform("u_CameraPos", MainCamera.Position);

	TextRenderer = new mst::TextRenderer();
	TextRenderer->Init(20, "Data/caviardreamsbold.ttf");
	TextRenderer->UseProgram();
	TextRenderer->SetUniform("u_CameraPos", MainCamera.Position);

	UserResize();

	srand(timer.startoffset);

	Player1Paddle.x = PaddleWallDistance;
	Player2Paddle.x = ScreenSize.x - PaddleWallDistance - PaddleSize.x;
	Player1Paddle.y = Player2Paddle.y = ScreenCenter.y - (PaddleSize.y*0.5f);

	Ball = ScreenCenter;
	BallSize = {10.f,10.f};
	BallMoveDirection = GetNewBallDirection();

	return true;
}

void MyGame::UserResize()
{
	if (QuadRenderer != nullptr)
	{
		QuadRenderer->UseProgram();
		QuadRenderer->SetUniform("u_WorldSize", ScreenSize);
	}

	if (TextRenderer != nullptr)
	{
		TextRenderer->UseProgram();
		TextRenderer->SetUniform("u_WorldSize", ScreenSize);
	}
}

void MyGame::UserUpdate()
{
#ifndef PLATFORM_WEB
	if (IsKeyPressed(mst::Key::ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}
#endif

	if (IsKeyDown(mst::Key::W))
	{
		Player1Paddle.y += PaddMoveSpeed * GetDeltaTime();
		ClampXPositionUp(Player1Paddle, PaddleSize);
	}
	if (IsKeyDown(mst::Key::S))
	{
		Player1Paddle.y -= PaddMoveSpeed * GetDeltaTime();
		ClampXPositionDown(Player1Paddle);
	}

	Ball += BallMoveDirection * GetDeltaTime() * BallSpeed;
	BallSpeed += GetDeltaTime();
	BounceBall();

	if (Player2Paddle.y + (PaddleSize.y*.5f) < Ball.y)
	{
		Player2Paddle.y += PaddMoveSpeed * GetDeltaTime();
		ClampXPositionUp(Player2Paddle, PaddleSize);
	}
	else if (Player2Paddle.y + (PaddleSize.y*.5f) > Ball.y)
	{
		Player2Paddle.y -= PaddMoveSpeed * GetDeltaTime();
		ClampXPositionDown(Player2Paddle);
	}
}

void MyGame::UserRender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	QuadRenderer->StartRender();
	QuadRenderer->AddRect(Player1Paddle, PaddleSize);
	QuadRenderer->AddRect(Player2Paddle, PaddleSize);
	QuadRenderer->AddRect(Ball, BallSize);
	QuadRenderer->EndRender();
	
	TextRenderer->StartRender();
	TextRenderer->RenderText(GetFPSString(), v2f(0, ScreenSize.y-30));
	TextRenderer->EndRender();
}

void MyGame::ClampXPositionUp(v2f& pos, const v2f& size)
{
	if (pos.y > ScreenSize.y - size.y)
	{
		pos.y = ScreenSize.y - size.y;
	}
}

void MyGame::ClampXPositionDown(v2f& pos)
{
	if (pos.y < 0)
	{
		pos.y = 0;
	}
}

float MyGame::RandomFloat(float low, float high)
{
	return low + ((float)rand() / float(RAND_MAX))*((high-low)+low);
}

void MyGame::BounceBall()
{	
	if (Ball.y > ScreenSize.y - BallSize.y || Ball.y < 0.0f)
	{
		BallMoveDirection.y *= -1.f;
	}

	if (Ball.x < 0.0)
	{
		Ball = ScreenCenter;
		BallMoveDirection = GetNewBallDirection();
		Player2Score++;
	}
	if (Ball.x > ScreenSize.x - BallSize.x)
	{
		Ball = ScreenCenter;
		BallMoveDirection = GetNewBallDirection();
		Player1Score++;
	}
}

v2f MyGame::GetNewBallDirection()
{
	float Dir = RandomFloat(-1,1);
	return std::move(v2f(Dir, cosf(Dir)));
}

