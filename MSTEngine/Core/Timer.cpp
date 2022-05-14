#include "Timer.h"

#if defined _WIN64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
namespace mst
{ 
	Timer::Timer()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
		QueryPerformanceCounter((LARGE_INTEGER*)&startoffset);
		delta = 0.0f;
		time = 0.0;
	}
	
	void Timer::Update()
	{
		unsigned long long value;
		QueryPerformanceCounter((LARGE_INTEGER*)&value);
		//double curr = (double)(value - timer.startoffset) / (double)timer.frequency;
		float curr = (float)(value - startoffset) / (float)frequency;
	
		delta = curr - time;
		time = curr;
	}
}
#else
namespace mst
{
	Timer::Timer()
	{
		m_tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
		m_tp1 = m_tp2;

		// Our time per frame coefficient
		float fElapsedTime = elapsedTime.count();
		fLastElapsed = fElapsedTime;

		delta = 0.0f;
		time = 0.0;
	}

	void Timer::Update()
	{
		m_tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
		m_tp1 = m_tp2;

		// Our time per frame coefficient
		float fElapsedTime = elapsedTime.count();
		fLastElapsed = fElapsedTime;

		//do olc video stuff here
		delta = curr - time;
		time = curr;
	}
}
#endif
