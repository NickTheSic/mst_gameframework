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
	return new MyGame();
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

	QuadRenderer = new mst::QuadRenderer(1000);
	QuadRenderer->UseProgram();
	QuadRenderer->SetUniform("u_CameraPos", MainCamera.Position);
	QuadRenderer->SetUniform("u_CameraZoo", MainCamera.CurrentZoom);
	
	TextRenderer = new mst::TextRenderer();
	TextRenderer->Init(100, "Data/caviardreamsbold.ttf");
	TextRenderer->UseProgram();
	TextRenderer->SetUniform("u_CameraPos", MainCamera.Position);

	UserResize();

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
}

void MyGame::UserRender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	QuadRenderer->StartRender();

	QuadRenderer->EndRender();
	
	TextRenderer->StartRender();
	TextRenderer->RenderText(GetFPSString(), v2f(0, ScreenSize.y-30));
	TextRenderer->EndRender();
};
