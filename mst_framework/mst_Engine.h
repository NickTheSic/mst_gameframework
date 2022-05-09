#pragma once

#if defined _WIN64
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif
#include "mst_gl_header.h"
#include "mst_vec2.h"
#include "mst_keys.h"

#include <array>
#include <string>

namespace mst
{
	struct Timer
	{
		Timer();
		void Update();
		float delta;
		float time;
		#if defined _WIN64
		unsigned long long frequency;
		unsigned long long startoffset;
		#endif
	};

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

		bool CreateGLWindow(int width, int height);
		bool CoreUpdate();
		void SwapBuffers();
		void SetWindowTitle(std::string title);

		void HandleKey(Key key, bool Down);
		void HandleMouseButton(int button, bool Down);
		void HandleMouseMove(int x, int y);

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

		v2f MouseToScreen();

	//protected:
	#if defined _WIN64
		HWND  Window;
		HDC   Device;
		HGLRC Context;
		static LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
	#endif

		v2i PrevMousePos = {};
		v2i CurrMousePos = {};
		v2i ScreenSize   = {};

		Timer timer;

		std::array<ButtonState, 256> KeyStates;
		std::array<ButtonState, 3> MouseStates;

		bool IsRunning = true;
	};
}                  // namespace mst