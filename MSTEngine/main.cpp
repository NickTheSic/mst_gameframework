#define MST_GL_IMPLEMENTATION
#include "Core/Engine.h"
#include <iostream>

#if defined PLATFORM_WEB | __EMSCRIPTEN__

#include <emscripten.h>

void em_run(void* Engine)
{
	static_cast<mst::Engine*>(Engine)->CoreUpdate();
	static_cast<mst::Engine*>(Engine)->SwapBuffers();
	//static_cast<mst::Engine*>(Engine)->PollEvents();
}

#endif

extern "C" 
{
	int main(int argc, char* argv[])
	{
		mst::Engine* Engine = mst::Engine::CreateEngine();
		
		if (Engine->Construct(800, 600))
		{
			glClearColor(0.0f,0.0f,0.0f,1.0f);
			
			#if !defined PLATFORM_WEB || !defined __EMSCRIPTEN__
			while(!Engine->ShouldExit())
			{
				Engine->CoreUpdate();
				Engine->SwapBuffers();
				Engine->PollEvents();
			} 
			#else
			emscripten_set_main_loop_arg(em_run, Engine, 0, 1);
			#endif
		}
	
		//Engine Cleanup
		delete Engine;
		return 0;
	}
}