#pragma once

namespace ui
{

	template<typename T>
	struct tv2 {
		T x, y;
		template <typename U>
		void operator=(const tv2<U>& rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
		}
	};
	typedef tv2<int> v2i; typedef tv2<float> v2f;

	struct RGBA {
		unsigned char r, g, b, a;
		void operator=(const RGBA& rhs)
		{
			r = rhs.r;
			g = rhs.g;
			b = rhs.b;
			a = rhs.a;
		}
	};

	void Init();
	void RenderText(const char* Text, v2f& Position, float Scale = 1.0f);
}
