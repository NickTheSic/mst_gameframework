#include "Engine.h"

#define MST_KEY_OPERATOR_IMPLEMENTATION
#include "mstkeys.h"

#include <iostream>
#define dbglog(dbmsg) std::cout << dbmsg << std::endl;
#define dbgval(val)   std::cout << #val << ": " << (val) << std::endl;

namespace mst 
{
	void Engine::SwapBuffers()
	{
		::SwapBuffers(Device);
	}

	v2f Engine::GetMouseToScreen()
	{
		v2f mousePos(CurrMousePos);
		mousePos -= v2f(ScreenCenter) - MainCamera.Position;
		return std::move(mousePos);
	}

	v2f Engine::GetMouseMoveDelta()
	{
		return MouseMoveDelta;
	}
	
	void Engine::HandleKey(Key key, bool Down)
	{
		//dbglog(key << " Is " << Down);
		KeyStates[(unsigned int)key].DownState = Down;
	}

	bool Engine::IsKeyDown(Key key)
	{
		return KeyStates[(unsigned int)key].Held;
	}

	bool Engine::IsKeyPressed(Key key)
	{
		return KeyStates[(unsigned int)key].Pressed;
	}

	bool Engine::IsKeyReleased(Key key)
	{
		return KeyStates[(unsigned int)key].Released;
	}

	void Engine::HandleMouseButton(int button, bool Down)
	{
		MouseStates[button].DownState = Down;
	}

	bool Engine::IsMouseButtonDown(int button)
	{
		return MouseStates[button].Held;
	}

	bool Engine::IsMouseButtonPressed(int button)
	{
		return MouseStates[button].Pressed;
	}

	bool Engine::IsMouseButtonReleased(int button)
	{
		return MouseStates[button].Released;
	}

	void Engine::HandleMouseMove(int x, int y)
	{
		CurrMousePos.x = x;
		CurrMousePos.y = y;
	}

	void Engine::HandleMouseScroll(int scroll)
	{	
		CurrentFrameMouseScroll = scroll;
	}

	void Engine::SetWindowTitle(std::string title)
	{
	#ifdef UNICODE
	#ifdef __MINGW32__
		wchar_t* buffer = new wchar_t[s.length() + 1];
		mbstowcs(buffer, s.c_str(), s.length());
		buffer[s.length()] = L'\0';
	#else
		int count = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, NULL, 0);
		wchar_t* buffer = new wchar_t[count];
		MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, buffer, count);
	#endif
		SetWindowText(Window, buffer);

		delete[] buffer;
	#else
		SetWindowText(Window, s.c_str());
	#endif
	}
	
	bool Engine::PollEvents()
	{
		MSG Message;
		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if (Message.message != WM_QUIT)
			{
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	void Engine::CoreUpdate()
	{
		for (int i = 0; i < MouseStates.size(); i++)
		{
			MouseStates[i].Pressed = false;
			MouseStates[i].Released = false;
			if (MouseStates[i].DownState != MouseStates[i].PrevState)
			{
				if (MouseStates[i].DownState)
				{
					MouseStates[i].Pressed = !MouseStates[i].Held;
					MouseStates[i].Held = true;
				}
				else
				{
					MouseStates[i].Released = true;
					MouseStates[i].Held = false;
				}
			}

			MouseStates[i].PrevState = MouseStates[i].DownState;
		}
	
		for (int i = 0; i < KeyStates.size(); i++)
		{
			KeyStates[i].Pressed = false;
			KeyStates[i].Released = false;
			if (KeyStates[i].DownState != KeyStates[i].PrevState)
			{
				if (KeyStates[i].DownState)
				{
					KeyStates[i].Pressed = !KeyStates[i].Held;
					KeyStates[i].Held = true;
				}
				else
				{
					KeyStates[i].Released = true;
					KeyStates[i].Held = false;
				}
			}

			KeyStates[i].PrevState = KeyStates[i].DownState;
		}

		MouseScroll = CurrentFrameMouseScroll;
		CurrentFrameMouseScroll = 0;

		MouseMoveDelta = CurrMousePos - PrevMousePos;
		PrevMousePos = CurrMousePos;

		timer.Update();

		ShowFPS();

		// Custom Update
		UserUpdate();

		// Custom Render
		UserRender();
	}

	void Engine::ShowFPS()
	{
		static float TimedLoop;
		static int frameCount;

		TimedLoop += timer.delta;
		if (TimedLoop > 1.f)
		{
			double fps = double(frameCount) / TimedLoop;
			double msPerFrame = 1000.0 / fps;
			SetWindowTitle("FPS: " + std::to_string(fps) + " | " + std::to_string(msPerFrame) + "ms");
			TimedLoop -= 1.f;
		}
		frameCount++;
	}
	
	bool Engine::CreateGLWindow(int width, int height)
	{
		ScreenSize.x = width;
		ScreenSize.y = height;

		InitialScreenSize.x = width;
		InitialScreenSize.y = height;

		DWORD dwExStyle;
		DWORD dwStyle;

		RECT WindowRect;
		WindowRect.left = (long)0;
		WindowRect.right = (long)width;
		WindowRect.top = (long)0;
		WindowRect.bottom = (long)height;

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = mst::Engine::WindowProc;
		wc.hInstance = GetModuleHandle(0);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpszClassName = TEXT("MST_WINDOW_CLASS");
		wc.cbClsExtra = 0;                              // No extra window data.
		wc.cbWndExtra = 0;                              // No extra window data.
		wc.hIcon = LoadIcon(0, IDI_WINLOGO);            // Load the default icon.
		wc.hCursor = LoadCursor(0, IDC_ARROW);          // Load the arrow pointer.
		wc.hbrBackground = nullptr;                     // No background required for GL.
		wc.lpszMenuName = nullptr;                      // We don't want a menu.
	
		if (!RegisterClassEx(&wc))
		{
			return false;
		}

		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle   = WS_OVERLAPPEDWINDOW;

		AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);
	
		Window = CreateWindowEx
		(
			dwExStyle,
			wc.lpszClassName,
			TEXT("MST"),
			dwStyle,
			0, 0, 
			WindowRect.right - WindowRect.left,
			WindowRect.bottom - WindowRect.top,
			NULL, NULL, GetModuleHandle(0),
			this
		);
	
		if (Window == nullptr)
		{
			return false;
		}
	
		Device = GetDC(Window);
		if (Device == nullptr)
		{
			return false;
		}
	
		BYTE colorbits = 32;
		BYTE zbits = 31;
		BYTE stencilbits = 1;
		PIXELFORMATDESCRIPTOR pfd =  // pfd tells Windows how we want things to be.
		{
			sizeof(PIXELFORMATDESCRIPTOR),  // Size of this pixel format descriptor.
			1,                              // Version number.
			PFD_DRAW_TO_WINDOW |            // Format must support window.
			PFD_SUPPORT_OPENGL |            // Format must support opengl.
			PFD_DOUBLEBUFFER,               // Must support double buffering.
			PFD_TYPE_RGBA,                  // Request an rgba format.
			(BYTE)colorbits,                // Select our color depth.
			0, 0, 0, 0, 0, 0,               // Color bits ignored.
			0,                              // No alpha buffer.
			0,                              // Shift bit ignored.
			0,                              // No accumulation buffer.
			0, 0, 0, 0,                     // Accumulation bits ignored.
			(BYTE)zbits,                    // Bits for z-buffer (depth buffer).
			(BYTE)stencilbits,              // Stencil bits.
			0,                              // No auxiliary buffer.
			PFD_MAIN_PLANE,                 // Main drawing layer.
			0,                              // Reserved.
			0, 0, 0                         // Layer masks ignored.
		};
	
		unsigned int PixelFormat = ChoosePixelFormat(Device, &pfd);
	
		if (!(PixelFormat)) // Did Windows find a matching pixel format?
		{
			return false;
		}
	
		if (!SetPixelFormat(Device, PixelFormat, &pfd))
		{
			return false;
		}
	
		Context = wglCreateContext(Device);
		if (Context == nullptr)
		{
			return false;
		}
	
		if (!wglMakeCurrent(Device, Context))
		{
			return false;
		}

		// Get the name of the video card.
		{
			char* rendererString;
			rendererString = (char*)glGetString(GL_RENDERER);
			if (rendererString) dbglog(rendererString);
		}
	
		ShowWindow(Window, SW_SHOW);
		SetForegroundWindow(Window);
		SetFocus(Window);
	
		// After we create our Window we can try to load the extensions
		return LoadGLExtensions();
	}
	
	LRESULT CALLBACK Engine::WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = {};
	
		mst::Engine* UserEngine = (mst::Engine*)(GetWindowLongPtr(window, GWLP_USERDATA));
	
		const int _width = LOWORD(lParam);
		const int _height = HIWORD(lParam);
	
		switch (msg)
		{
		case WM_NCCREATE:
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			UserEngine = (mst::Engine*)pcs->lpCreateParams;
			::SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)(UserEngine));
			result = 1;
		} break;
	
		case WM_KEYDOWN:
		{
			UserEngine->HandleKey((mst::Key)wParam, true);
		} break;
	
		case WM_KEYUP:
		{
			UserEngine->HandleKey((mst::Key)wParam, false);
		} break;
	
		case WM_MOUSEMOVE:
		{
			const int _mousex = LOWORD(lParam);
			const int _mousey = HIWORD(lParam);
			UserEngine->HandleMouseMove(_mousex, UserEngine->ScreenSize.y - _mousey);
		} break;
	
		case WM_LBUTTONDOWN:
		{
			UserEngine->HandleMouseButton(0, true);
		} break;
	
		case WM_LBUTTONUP:
		{
			UserEngine->HandleMouseButton(0, false);
		} break;
	
		case WM_RBUTTONDOWN:
		{
			UserEngine->HandleMouseButton(1, true);
		} break;
	
		case WM_RBUTTONUP:
		{
			UserEngine->HandleMouseButton(1, false);
		} break;
	
		case WM_MOUSEWHEEL:
		{
			const int scroll = GET_WHEEL_DELTA_WPARAM(wParam);
			UserEngine->HandleMouseScroll(scroll);
		} break;

		case WM_SIZE:
		{
			glViewport(0, 0, _width, _height);

			UserEngine->ScreenSize.x = _width;
			UserEngine->ScreenSize.y = _height;

			UserEngine->ScreenCenter.x = _width*0.5f;
			UserEngine->ScreenCenter.y = _height*0.5f;

			UserEngine->UserResize();
		} break;
	
		case WM_DESTROY:
		{
			MessageBox(UserEngine->Window, TEXT("Destroyed"), TEXT("Destroyed"), MB_OK);
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		} break;
	
		default:
		{
			result = DefWindowProc(window, msg, wParam, lParam);
		} break;
		}
	
		return result;
	}
} //namespace mst