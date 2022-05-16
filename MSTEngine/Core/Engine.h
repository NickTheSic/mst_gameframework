#pragma once

#if defined _WIN64
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#if defined PLATFORM_WEB || __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#include <emscripten/HTML5.h>
#endif

#include "mstgl.h"

#include "mstkeys.h"
#include "mstglm.h"

#include "Camera.h"
#include "Timer.h"

#include <array>
#include <string>

namespace mst
{
	struct ButtonState
	{
		bool Held      : 1;
		bool Pressed   : 1;
		bool Released  : 1;
		bool DownState : 1;
		bool PrevState : 1;
	};

	class Engine
	{
	public:
		virtual ~Engine() = default;

		//Implemented by use game
		static class Engine* CreateEngine();

		virtual bool UserStartup() {return false; /*Forces user to override?*/ };
		virtual void UserUpdate(){};
		virtual void UserRender(){};
		virtual void UserResize(){};

		bool Construct(int width, int height)
		{
			bool success = CreateGLWindow(width, height) & UserStartup();
			return success;
		}

		bool CreateGLWindow(int width, int height);
		void PollEvents();
		bool ShouldExit();
		void CoreUpdate();
		void SwapBuffers();
		void SetWindowTitle(std::string title);

		void HandleKey(Key key, bool Down);
		void HandleMouseButton(int button, bool Down);
		void HandleMouseMove(int x, int y);
		void HandleMouseScroll(int scroll);

		bool IsKeyDown(Key key);
		bool IsKeyPressed(Key key);
		bool IsKeyReleased(Key key);
		bool IsMouseButtonDown(int button);
		bool IsMouseButtonPressed(int button);
		bool IsMouseButtonReleased(int button);

		inline float GetDeltaTime()
		{
			return timer.delta;
		}

		v2f GetMouseMoveDelta();
		v2f GetMouseToScreen();
		v2f GetMousePosition();

		void ShowFPS();

	protected:

	#if defined _WIN64
		HWND  Window;
		HDC   Device;
		HGLRC Context;
		static LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
	#endif

	#if defined __EMSCRIPTEN__ || PLATFORM_WEB
		EGLDisplay Display;
		EGLConfig  Config;
		EGLContext Context;
		EGLSurface Surface;
	#endif

		v2i PrevMousePos      = {};
		v2i CurrMousePos      = {};
		v2f MouseMoveDelta    = {};

		int MouseScroll;
		int CurrentFrameMouseScroll;

		v2i ScreenSize        = {};
		v2i ScreenCenter      = {};
		v2i InitialScreenSize = {};

		Camera MainCamera;

		Timer timer;

		std::array<ButtonState, 256> KeyStates;
		std::array<ButtonState, 3> MouseStates;
		//std::array<ButtonState, Controllerinputs> ControllerInput;?

		bool bShouldExit = false;
	};
} // namespace mst