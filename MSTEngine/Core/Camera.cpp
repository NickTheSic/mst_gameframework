#include "Camera.h"

namespace mst
{ 
	bool Camera::Zoom(float Val)
	{
		if (Val == 0.0f)
		{
			return false;
		}
	
		CurrentZoom += Val;
		return true;
	}
	
	bool Camera::MoveCamera(const v2f& dir)
	{
		if (dir == v2f{ 0 })
		{
			return false;
		}
	
		Position += dir;
	
		return true;
	}
}