#pragma once

#if defined __EMSCRITPEN__ || defined PLATFORM_WEB
#define GLM_FORCE_PURE
#endif

#include <glm/glm.hpp>

typedef glm::vec2   v2f;
typedef glm::ivec2  v2i;
typedef glm::vec3   v3f;
typedef glm::u8vec3 Color;


#if 0

	template<typename T>
	struct tv2
	{
		union
		{
			T x, w, u;
		};
		union
		{
			T y, h, v;
		};
	};

	template<typename T>
	struct tv3
	{
		union
		{
			T x, w, r;
		};
		union
		{
			T y, h, g;
		};
		union
		{
			T z, d, b;
		}
	};


#endif
