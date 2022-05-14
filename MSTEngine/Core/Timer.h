#pragma once

#ifdef _WIN64
#define CHRONO_WAY 0
#else
#define CHRONO_WAY 1
#endif

#if CHRONO_WAY
#include <chrono>
#endif

namespace mst
{
	struct Timer
	{
		Timer();
		void Update();
		float delta;
		float time;
		#if !CHRONO_WAY
		unsigned long long frequency;
		unsigned long long startoffset;
		#else
		std::chrono::time_point<std::chrono::system_clock> tp1, tp2;
		#endif
	};
}
