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
		QuadRenderer = new mst::QuadRenderer(500);
		mst::InitColourShader(QuadRenderer->shaderProgram);

		for (int x = 0; x < ScreenSize.x; x+=10)
		{
			for (int y = 0; y < ScreenSize.y; y += 10)
			{
				GridRectPositions.push_back({ x, y });
			}
		}

		srand(timer.startoffset);
		for (int i = 0; i < 10; i++)
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

		if (IsMouseButtonPressed(0))
			MousePositions.push_back(MouseToScreen());

		if (IsKeyPressed(mst::Key::ESCAPE))
		{
			PostQuitMessage(0);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		QuadRenderer->StartRender();

		for (auto& pos : GridRectPositions)
		{
			QuadRenderer->AddRect(pos, { 10.f, 10.f }, RandomColours[rand() % RandomColours.size()]);
		}

		for (auto& pos : MousePositions)
		{
			QuadRenderer->AddCenteredQuad(pos, { 10.f, 10.f }, {0,0,0});
		}

		QuadRenderer->EndRender();
	};

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