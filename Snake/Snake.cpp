#include "Snake.h"

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
	MyGame* g = new MyGame(200, 200);
	return g;
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

	QuadRenderer = new mst::QuadRenderer(100);
	QuadRenderer->UseProgram();
	QuadRenderer->SetUniform("u_CameraPos", MainCamera.Position);
	
	TextRenderer = new mst::TextRenderer();
	TextRenderer->Init(20, "Data/caviardreamsbold.ttf");
	TextRenderer->UseProgram();
	TextRenderer->SetUniform("u_CameraPos", MainCamera.Position);

	UserResize();

	srand(timer.startoffset);

	Size = v2f(10,10);
	Dir = v2f(0,1);
	Apple = GetRandomPosOnScreen();
	Snake.push_back(GetRandomPosOnScreen());

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

	CurrentUpdate += GetDeltaTime();
	
	if (CurrentUpdate > UpdateInterval)
	{ 
		CurrentUpdate = 0.0f;

		for (int i = Snake.size()-1; i > 0; --i)
		{
			Snake[i] = Snake[i-1];
		}
		Snake[0] += (Dir * Size);
		if (Snake[0].x < 0) Snake[0].x = ScreenSize.x - Size.x;
		if (Snake[0].y < 0) Snake[0].y = ScreenSize.y - Size.y;
		if (Snake[0].x > ScreenSize.x) Snake[0].x = 0 + Size.x;
		if (Snake[0].y > ScreenSize.y) Snake[0].y = 0 + Size.y;

		if (Snake[0].x + Size.x > Apple.x
			&& Snake[0].x < Apple.x + Size.x
			&& Snake[0].y < Apple.y + Size.y
			&& Snake[0].y + Size.y > Apple.y)
		{
			Snake.push_back(Snake[Snake.size()-1]);
			Apple = GetRandomPosOnScreen();
		}
	}

	if (IsKeyPressed(mst::Key::W) && Dir != v2f(0,-1))
	{
		Dir = v2f(0,1);
	}
	if (IsKeyPressed(mst::Key::S) && Dir != v2f(0, 1))
	{
		Dir = v2f(0, -1);
	}
	if (IsKeyPressed(mst::Key::A) && Dir != v2f(1, 0))
	{
		Dir = v2f(-1, 0);
	}
	if (IsKeyPressed(mst::Key::D) && Dir != v2f(-1, 0))
	{
		Dir = v2f(1, 0);
	}
}

void MyGame::UserRender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	QuadRenderer->StartRender();
	QuadRenderer->AddRect(Apple, Size, Color(255,0,0));

	for (auto& seg : Snake)
	{
		QuadRenderer->AddRect(seg, Size, Color(55, 255, 0));
	}

	QuadRenderer->EndRender();
	
	TextRenderer->StartRender();
	TextRenderer->RenderText(std::to_string(Snake.size()), v2f(0, ScreenSize.y-30));
	TextRenderer->EndRender();
}
v2f MyGame::GetRandomPosOnScreen()
{
	return v2f(GetRandomFloat(0, ScreenSize.x - Size.x), 
			   GetRandomFloat(0, ScreenSize.y - Size.y));
}
