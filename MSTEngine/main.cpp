#define MST_GL_IMPLEMENTATION
#include "Core/Engine.h"
#include "Renderer/BatchRenderer.h"
#include "Renderer/TextRenderer.h"
#include <vector>

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

class MyGame : public mst::Engine
{
public:
	
	~MyGame()
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

	virtual bool UserStartup() 
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
		
		MainCamera.Position = ScreenSize/2;

		QuadRenderer = new mst::QuadRenderer(14400/6);
		mst::InitColourShader(QuadRenderer->rd.shaderProgram);

		TextRenderer = new mst::TextRenderer(100, "leadcoat.ttf");

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

		GridSize = ScreenSize/SquareSizes;
		for (int y = 0; y < ScreenSize.y; y += SquareSizes)
		{
			for (int x = 0; x < ScreenSize.x; x+= SquareSizes)
			{
				GridRectPositions.push_back({ x, y });

				RandomColours.push_back({ rand(), rand(), rand() });
			}
		}

		UserResize();

		return true; 
	}

	virtual void UserResize()
	{
		if (QuadRenderer != nullptr)
		{ 
			GLint worldSizeLoc = glGetUniformLocation(QuadRenderer->rd.shaderProgram, "u_WorldSize");
			if (worldSizeLoc != -1)
			{
				v2f screenSize(ScreenSize);
				glUniform2fv(worldSizeLoc, 1, &screenSize[0]);
			}
		}
	}

	virtual void UserUpdate() 
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
			*MoveableGridPiece = GetMouseToScreen() - v2f{SquareSizes*0.5, SquareSizes * 0.5};
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
		if (MainCamera.Zoom(MouseScroll*GetDeltaTime()))
		{
			GLint cameraZoomLoc = glGetUniformLocation(QuadRenderer->rd.shaderProgram, "u_CameraZoom");
			if (cameraZoomLoc != -1)
			{
				glUniform1f(cameraZoomLoc, MainCamera.CurrentZoom);
			}
		}
	}

	virtual void UserRender()
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
			QuadRenderer->AddCenteredQuad(pos, { SquareSizes, SquareSizes }, Color{0,0,0});
		}

		QuadRenderer->EndRender();
	};

	v2i WorldSpaceToIndex(const v2f& WorldCoord)
	{
		v2i Coord;

		Coord.x = (int)(WorldCoord.x/SquareSizes)%GridSize.x;
		Coord.y = (int)(WorldCoord.y/SquareSizes)%GridSize.y;

		return std::move(Coord);
	}

	int SquareSizes = 20;
	float Tick = 0.0f;
	mst::QuadRenderer* QuadRenderer = nullptr;
	mst::TextRenderer* TextRenderer = nullptr;
	std::vector<v2f> MousePositions;
	std::vector<v2f> GridRectPositions;
	std::vector<Color> RandomColours;
	v2i GridSize;

	v2f* MoveableGridPiece = nullptr;
};


extern "C" 
{
	int main(int argc, char* argv[])
	{
		mst::Engine* Engine = new MyGame();
	
		if (Engine->Construct(800, 600))
		{
			glClearColor(0.0f,0.0f,0.0f,1.0f);
	
			while (Engine->PollEvents())
			{
				Engine->CoreUpdate();
				Engine->SwapBuffers();
			}
		}
	
		//Engine Cleanup
		delete Engine;
		return 0;
	}
}