#include "Demo.h"

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

	if (FirstSpriteSheetGenerator)
	{
		delete FirstSpriteSheetGenerator;
	}
}

bool MyGame::UserStartup()
{
	std::cout << "User Startup!" << std::endl;
	glClearDepth(1.0f);
	// Set the polygon winding to front facing for the left handed system.
	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//glEnable(GL_DEPTH);

	glClearColor(1.f,0.f,0.f,1.f);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	MainCamera.Position = ScreenSize / 2;

	QuadRenderer = new mst::QuadRenderer(1000);
	QuadRenderer->UseProgram();
	QuadRenderer->SetUniform("u_CameraPos", MainCamera.Position);
	QuadRenderer->SetUniform("u_CameraZoo", MainCamera.CurrentZoom);
	
	TextRenderer = new mst::TextRenderer();
	TextRenderer->Init(100, "Data/caviardreamsbold.ttf");
	TextRenderer->UseProgram();
	TextRenderer->SetUniform("u_CameraPos", MainCamera.Position);
	
	TextRenderer2 = new mst::TextRenderer(26, "Data/leadcoat.ttf");
	TextRenderer2->UseProgram();
	TextRenderer2->SetUniform("u_CameraPos", MainCamera.Position);

	FirstSpriteSheetGenerator = new mst::SpriteSheetGeneratorRenderer();
	std::vector<std::string> Files;
	Files.reserve(179);
	for (int i = 0; i < 179; i++)
	{
		std::string path = "Data/kenney_platformer/tile_0";

		int value = i;
		std::vector<int> values;
		values.reserve(3);
		do
		{
			values.push_back(value%10);
			value/=10;
		}
		while (value > 0);

		switch (values.size())
		{
			case 1: values.push_back(0);
			case 2: values.push_back(0);
		}

		for (int i = 0; i < 3; i++)
		{
			path.append(std::to_string(values[2-i]));
		}

		path.append(".png");
		Files.push_back(path.c_str());
	}
	FirstSpriteSheetGenerator->Init(100, Files);
	FirstSpriteSheetGenerator->UseProgram();
	FirstSpriteSheetGenerator->SetUniform("u_CameraPos", MainCamera.Position);

	GridSize = ScreenSize / SquareSizes;
	for (int y = 0; y < ScreenSize.y; y += SquareSizes)
	{
		for (int x = 0; x < ScreenSize.x; x += SquareSizes)
		{
			GridRectPositions.push_back({ x, y });

			RandomColours.push_back({ rand(), rand(), rand() });
		}
	}

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

	if (TextRenderer2 != nullptr)
	{
		TextRenderer2->UseProgram();
		TextRenderer2->SetUniform("u_WorldSize", ScreenSize);
	}

	if (FirstSpriteSheetGenerator != nullptr)
	{
		FirstSpriteSheetGenerator->UseProgram();
		FirstSpriteSheetGenerator->SetUniform("u_WorldSize", ScreenSize);
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

	v2f cameraMove(0,0);

	float speed = 80;

	if (IsKeyDown(mst::Key::W))
	{
	    cameraMove.y -= speed * GetDeltaTime();
	}
	if (IsKeyDown(mst::Key::S))
	{
		cameraMove.y += speed * GetDeltaTime();
	}
	if (IsKeyDown(mst::Key::A))
	{
		cameraMove.x += speed * GetDeltaTime();
	}
	if (IsKeyDown(mst::Key::D))
	{
		cameraMove.x -= speed * GetDeltaTime();
	}

	if (IsMouseButtonPressed(0))
	{
		MousePositions.push_back(GetMouseToScreen());

		v2i Coord = WorldSpaceToIndex(GetMouseToScreen());
		int idx = (Coord.y * GridSize.x) + Coord.x;
		if (idx > 0 && idx < GridSize.x * GridSize.y)
			MoveableGridPiece = &GridRectPositions[idx];
	}

	if (IsMouseButtonReleased(0))
	{
		MoveableGridPiece = nullptr;
	}

	if (IsMouseButtonDown(0) && MoveableGridPiece != nullptr)
	{
		*MoveableGridPiece = GetMouseToScreen() - v2f{ SquareSizes * 0.5, SquareSizes * 0.5 };
	}

	if (MainCamera.MoveCamera(cameraMove))
	{
		QuadRenderer->UseProgram();
		QuadRenderer->SetUniform("u_CameraPos", MainCamera.Position);
	}
	if (MainCamera.Zoom(MouseScroll * GetDeltaTime()))
	{
		QuadRenderer->UseProgram();
		QuadRenderer->SetUniform("u_CameraZoom", MainCamera.CurrentZoom);
	}
}

void MyGame::UserRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QuadRenderer->StartRender();
	int idx = 0;
	for (auto& pos : GridRectPositions)
	{
		//QuadRenderer->AddRect(pos, { SquareSizes, SquareSizes }, RandomColours[idx]);
		idx++;
	}
	for (auto& pos : MousePositions)
	{
		QuadRenderer->AddCenteredQuad(pos, { SquareSizes, SquareSizes }, Color{ 255,255,255 });
	}
	QuadRenderer->AddCenteredQuad(GetMouseToScreen(), { SquareSizes, SquareSizes }, Color{ 255,255,255 });
	QuadRenderer->EndRender();
	
	TextRenderer->StartRender();
	TextRenderer->RenderText("ABCDEFGHIJKLMNOPQRSTUVWXYZ", v2f(30,45));
	TextRenderer->RenderTextFromRight("ABCDEFGHIJKLMNOPQRSTUVWXYZ", v2f(ScreenSize.x, 100));
	TextRenderer->RenderText(GetFPSString(), v2f(0, ScreenSize.y-30));
	TextRenderer->EndRender();
	
	TextRenderer2->StartRender();
	TextRenderer2->RenderText("ABCDEFGHIJKLMNOPQRSTUVWXYZ", v2f(0, 5));
	TextRenderer2->EndRender();

	FirstSpriteSheetGenerator->StartRender();
	//FirstSpriteSheetGenerator->QuickRenderSansImage(v2f(0,ScreenCenter.y));
	//FirstSpriteSheetGenerator->QuickRender(v2f(0,ScreenCenter.y));
	idx = 1;
	for (auto& pos : GridRectPositions)
	{
	FirstSpriteSheetGenerator->RenderSpriteAtIndex(idx, pos);
		idx++;
		idx=idx%179;
	}
	FirstSpriteSheetGenerator->EndRender();
};

v2i MyGame::WorldSpaceToIndex(const v2f& WorldCoord)
{
	v2i Coord;

	Coord.x = (int)(WorldCoord.x / SquareSizes) % GridSize.x;
	Coord.y = (int)(WorldCoord.y / SquareSizes) % GridSize.y;

	return std::move(Coord);
}
