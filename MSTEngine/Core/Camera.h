#pragma once

#include "mstglm.h"

namespace mst
{ 
	struct Camera
	{
		v2f Position = {};
		float CurrentZoom = 1.f;
	
		bool Zoom(float Val);
		bool MoveCamera(const v2f& dir);
	};
}
