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
}

bool MyGame::UserStartup()
{
	glClearDepth(1.0f);
	// Set the polygon winding to front facing for the left handed system.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_BLEND
		| GL_CULL_FACE
		| GL_DEPTH_TEST
	);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MainCamera.Position = ScreenSize / 2;

	QuadRenderer = new mst::QuadRenderer((ScreenSize.y*ScreenSize.x)/SquareSizes);
	mst::InitColourShader(QuadRenderer->rd.shaderProgram);
	GLint cameraPosLoc = glGetUniformLocation(QuadRenderer->rd.shaderProgram, "u_CameraPos");
	if (cameraPosLoc != -1)
	{
		glUniform2fv(cameraPosLoc, 1, &MainCamera.Position[0]);
	}
	GLint cameraZoomLoc = glGetUniformLocation(QuadRenderer->rd.shaderProgram, "u_CameraZoom");
	if (cameraZoomLoc != -1)
	{
		glUniform1f(cameraZoomLoc, MainCamera.CurrentZoom);
	}

	TextRenderer = new mst::QuadRenderer(SquareSizes); //new mst::TextRenderer(5, "Data/leadcoat.ttf");
	////mst::InitTextShader(TextRenderer->rd.shaderProgram);
	mst::InitColourShader(TextRenderer->rd.shaderProgram);
	GLint cameraPosLocText = glGetUniformLocation(TextRenderer->rd.shaderProgram, "u_CameraPos");
	if (cameraPosLocText != -1)
	{
		glUniform2fv(cameraPosLocText, 1, &MainCamera.Position[0]);
	}

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
		glUseProgram(QuadRenderer->rd.shaderProgram);
		GLint worldSizeLoc = glGetUniformLocation(QuadRenderer->rd.shaderProgram, "u_WorldSize");
		if (worldSizeLoc != -1)
		{
			v2f screenSize(ScreenSize);
			glUniform2fv(worldSizeLoc, 1, &screenSize[0]);
		}
	}

	if (TextRenderer != nullptr)
	{
		glUseProgram(TextRenderer->rd.shaderProgram);
		GLint worldSizeLoc = glGetUniformLocation(TextRenderer->rd.shaderProgram, "u_WorldSize");
		if (worldSizeLoc != -1)
		{
			v2f screenSize(ScreenSize);
			glUniform2fv(worldSizeLoc, 1, &screenSize[0]);
		}
	}
}

void MyGame::UserUpdate()
{
	Tick += GetDeltaTime();
	if (Tick > 3.0f)
	{
		dbgval(QuadRenderer->rd.DrawsPerFrame);
		dbgval(MousePositions.size() + GridRectPositions.size());
		Tick -= 3.0f;
	}

	//MousePositions.push_back(GetMouseToScreen());

	if (IsKeyPressed(mst::Key::ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	if (IsMouseButtonPressed(0))
	{
		v2i Coord = WorldSpaceToIndex(GetMouseToScreen());
		int idx = (Coord.y * GridSize.x) + Coord.x;
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

	//v2f cameraMove(0);
	//if (MainCamera.MoveCamera(cameraMove))
	//{
	//	GLint cameraPosLoc = glGetUniformLocation(QuadRenderer->rd.shaderProgram, "u_CameraPos");
	//	if (cameraPosLoc != -1)
	//	{
	//		glUniform2fv(cameraPosLoc, 1, &MainCamera.Position[0]);
	//	}
	//}
	if (MainCamera.Zoom(MouseScroll * GetDeltaTime()))
	{
		GLint cameraZoomLoc = glGetUniformLocation(QuadRenderer->rd.shaderProgram, "u_CameraZoom");
		if (cameraZoomLoc != -1)
		{
			glUniform1f(cameraZoomLoc, MainCamera.CurrentZoom);
		}
	}
}

void MyGame::UserRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QuadRenderer->StartRender();

	int idx = 0;
	for (auto& pos : GridRectPositions)
	{
		QuadRenderer->AddRect(pos, { SquareSizes, SquareSizes }, RandomColours[idx]);
		idx++;
	}

	for (auto& pos : MousePositions)
	{
		QuadRenderer->AddCenteredQuad(pos, { SquareSizes, SquareSizes }, Color{ 255,255,255 });
	}

	QuadRenderer->EndRender();

	TextRenderer->StartRender();
	TextRenderer->AddRect(ScreenCenter, {30, 30}, Color{0,255,255});
	TextRenderer->EndRender();
};

v2i MyGame::WorldSpaceToIndex(const v2f& WorldCoord)
{
	v2i Coord;

	Coord.x = (int)(WorldCoord.x / SquareSizes) % GridSize.x;
	Coord.y = (int)(WorldCoord.y / SquareSizes) % GridSize.y;

	return std::move(Coord);
}
