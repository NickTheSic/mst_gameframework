#define MST_GL_IMPLEMENTATION
#include "Core/Engine.h"

extern "C" 
{
	int main(int argc, char* argv[])
	{
		mst::Engine* Engine = mst::Engine::CreateEngine();
	
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