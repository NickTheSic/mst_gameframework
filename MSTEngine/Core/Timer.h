#pragma once

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
		#else
		//std::chrono::time tp1, tp2;
		#endif
	};
}
