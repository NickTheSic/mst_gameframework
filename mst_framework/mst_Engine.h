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

	struct Camera
	{
		v2f Position = {};
		float CurrentZoom = 1.f;

		bool Zoom(float Val)
		{
			if (Val == 0.0f)
			{
				return false;
			}

			CurrentZoom += Val;
			return true;
		}

		bool MoveCamera(const v2f& dir)
		{
			if (dir == v2f{ 0 })
			{
				return false;
			}

			Position += dir;

			return true;
		}
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

		virtual bool UserStartup() {return true;};
		virtual void UserUpdate(){};
		virtual void UserResize(){};

		bool Construct(int width, int height)
		{
			bool success = CreateGLWindow(width, height) & UserStartup();
			return success;
		}

		bool CreateGLWindow(int width, int height);
		bool PollEvents();
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

	protected:
	#if defined _WIN64
		HWND  Window;
		HDC   Device;
		HGLRC Context;
		static LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
	#endif

		v2i PrevMousePos      = {};
		v2i CurrMousePos      = {};
		v2f MouseDelta        = {};
		v2i ScreenSize        = {};
		v2i ScreenCenter      = {};
		v2i InitialScreenSize = {};
		Camera MainCamera;

		Timer timer;

		int MouseScroll;
		int PrevMouseScroll;

		std::array<ButtonState, 256> KeyStates;
		std::array<ButtonState, 3> MouseStates;

		bool IsRunning = true;
	};
}                  // namespace mst