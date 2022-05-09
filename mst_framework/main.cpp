#define MST_GL_IMPLEMENTATION
#include "mst_Engine.h"
#include "mst_BatchRenderer.h"
#include <vector>

#define dbgval(val) std::cout << #val << ": " << val << std::endl;
#define dbglog(msg) std::cout << msg << std::endl;

int main(int argc, char** argv)
{
	mst::Engine* Engine = new mst::Engine();

	if (Engine->CreateGLWindow(600, 600))
	{
		mst::QuadRenderer QuadRenderer(500);
		mst::InitColourShader(QuadRenderer.shaderProgram);

		std::vector<mst::v2f> Positions;

		glClearColor(0.25f,0.5f,0.7f,1.0f);

		float Tick = 0.0f;

		while (Engine->CoreUpdate())
		{
			Tick += Engine->GetDeltaTime();
			if (Tick > 3.0f)
			{	
				dbgval(QuadRenderer.DrawsPerFrame);
				dbgval(Positions.size());
				Tick -= 3.0f;
			}

			if (Engine->IsMouseButtonPressed(0))
			{ 
				Positions.push_back(Engine->MouseToScreen());
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			QuadRenderer.StartRender();
			for (auto& pos : Positions)
			{
				QuadRenderer.AddCenteredQuad(pos, {0.025f,0.025f});
			}
			QuadRenderer.EndRender();

			Engine->SwapBuffers();
		}
	}

	//Engine Cleanup
	delete Engine;

	return 0;
}