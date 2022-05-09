#define MST_GL_IMPLEMENTATION
#include "mst_Engine.h"
#include "mst_BatchRenderer.h"
#include <vector>

#define dbgval(val) std::cout << #val << ": " << val << std::endl;
#define dbglog(msg) std::cout << msg << std::endl;

class MyGame : public mst::Engine
{
public:
	
	~MyGame()
	{
		if (QuadRenderer)
		{
			delete QuadRenderer;
		}
	}

	virtual bool UserStartup() 
	{ 
		MainCamera.Position = ScreenSize/2;

		QuadRenderer = new mst::QuadRenderer(4000);
		mst::InitColourShader(QuadRenderer->shaderProgram);

		GLint cameraPosLoc = glGetUniformLocation(QuadRenderer->shaderProgram, "u_CameraPos");
		if (cameraPosLoc != -1)
		{
			glUniform2fv(cameraPosLoc, 1, &MainCamera.Position[0]);
		}
		GLint cameraZoomLoc = glGetUniformLocation(QuadRenderer->shaderProgram, "u_CameraZoom");
		if (cameraZoomLoc != -1)
		{
			glUniform1f(cameraZoomLoc, MainCamera.CurrentZoom);
		}

		for (int x = 0; x < ScreenSize.x; x+= SquareSizes)
		{
			for (int y = 0; y < ScreenSize.y; y += SquareSizes)
			{
				GridRectPositions.push_back({ x, y });
			}
		}

		srand(timer.startoffset);
		for (int i = 0; i < (ScreenSize.x- SquareSizes) * (ScreenSize.y- SquareSizes); i++)
		{
			RandomColours.push_back({rand(), rand(), rand()});
		}

		UserResize();

		return true; 
	}

	virtual void UserResize()
	{
		if (QuadRenderer != nullptr)
		{ 
			GLint worldSizeLoc = glGetUniformLocation(QuadRenderer->shaderProgram, "u_WorldSize");
			if (worldSizeLoc != -1)
			{
				mst::v2f screenSize(ScreenSize);
				glUniform2fv(worldSizeLoc, 1, &screenSize[0]);
			}
		}
	}

	virtual void UserUpdate() 
	{
		Tick += GetDeltaTime();
		if (Tick > 3.0f)
		{
			dbgval(QuadRenderer->DrawsPerFrame);
			dbgval(MousePositions.size() + GridRectPositions.size());
			Tick -= 3.0f;
		}

		//MousePositions.push_back(GetMouseToScreen());

		if (IsKeyPressed(mst::Key::ESCAPE))
		{
			PostQuitMessage(0);
		}

		mst::v2f cameraMove(0);
		if (IsMouseButtonDown(0))
		{
			cameraMove = -GetMouseMoveDelta();
		}
		if (MainCamera.MoveCamera(cameraMove))
		{
			GLint cameraPosLoc = glGetUniformLocation(QuadRenderer->shaderProgram, "u_CameraPos");
			if (cameraPosLoc != -1)
			{
				glUniform2fv(cameraPosLoc, 1, &MainCamera.Position[0]);
			}
		}
		if (MainCamera.Zoom(MouseScroll*GetDeltaTime()))
		{
			GLint cameraZoomLoc = glGetUniformLocation(QuadRenderer->shaderProgram, "u_CameraZoom");
			if (cameraZoomLoc != -1)
			{
				glUniform1f(cameraZoomLoc, MainCamera.CurrentZoom);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		QuadRenderer->StartRender();

		int idx = 0;
		for (auto& pos : GridRectPositions)
		{
			QuadRenderer->AddRect(pos, { SquareSizes }, RandomColours[idx]);
			idx++;
		}

		for (auto& pos : MousePositions)
		{
			QuadRenderer->AddCenteredQuad(pos, { SquareSizes*SquareSizes }, mst::Color{0,0,0});
		}

		QuadRenderer->EndRender();
	};

	int SquareSizes = 5;
	float Tick = 0.0f;
	mst::QuadRenderer* QuadRenderer = nullptr;
	std::vector<mst::v2f> MousePositions;
	std::vector<mst::v2f> GridRectPositions;
	std::vector<mst::Color> RandomColours;
};


int main(int argc, char** argv)
{
	mst::Engine* Engine = new MyGame();

	if (Engine->Construct(600, 600))
	{
		glClearColor(0.25f,0.5f,0.7f,1.0f);

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