#include "Timer.h"

#include <iostream>

#if !CHRONO_WAY
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
		tp2 = std::chrono::system_clock::now();
		tp1 = tp2;

		delta = 0.0f;
		time = 0.0;
	}

	void Timer::Update()
	{
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;

		// Our time per frame coefficient
		float fElapsedTime = elapsedTime.count();

		//do olc video stuff here
		delta = fElapsedTime;

		std::cout << "Delta: " << delta << " time: " << time << " Elapsed: " << fElapsedTime << std::endl;
		time += fElapsedTime;
	}
}
#endif
