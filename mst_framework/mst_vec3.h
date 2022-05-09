#pragma once

#include <iostream>
#include "mst_vec2.h"

namespace mst
{
	template <class T>
	union tv3
	{
		tv3() :x{ 0 }, y{ 0 }, z{ 0 }{}
		tv3(T _x, T _y, T _z) :x{ _x }, y{ _y }, z{ _z }{}
		tv3(const tv2<T>& v) :x{ v.x }, y{ v.y }, z{ 0 }{}
		tv3(const tv3<T>& v) :x{ v.x }, y{ v.y }, z{ v.z }{}
		template<class U>
		tv3(const tv2<U>& v) : x{ (T)v.x }, y{ (T)v.y }, z{ 0 }{}
		template<class U>
		tv3(U _x, U _y, U _z) : x{ (T)_x }, y{ (T)_y }, z{(T)_z}{}
		struct { T x, y, z; };
		struct { T r, g, b; };
		struct { T w, h, d; };
		struct { tv2<T> xy; T Ignored0_; };
		struct { T Ignored1_; tv2<T> yz; };
		inline T& operator[](const int& i)
		{
			return dat[i];
		}
		inline const T& operator[](const int& i) const
		{
			return dat[i];
		}
		inline tv3<T> operator*=(const T& s)
		{
			x *= s; y *= s; z *= s;
			return (*this);
		}
		inline tv3<T> operator/=(const T& s)
		{
			T d = (T)1 / s;
			x *= d; y *= d; z *= d;
			return (*this);
		}
		inline tv3<T>& operator+=(const tv3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return (*this);
		}
		inline tv3<T>& operator-=(const tv3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return (*this);
		}
		inline tv3<T> operator=(const tv3<T>& v)
		{
			x=v.x;
			y=v.y;
			z=v.z;
			return (*this);
		}
	private:
		T dat[3];
	};

	template<class T, class U>
	inline tv3<T> operator*(tv3<T> vec, U s)
	{
		return tv3<T>(vec.x * (T)s, vec.y * (T)s, vec.z * (T)s);
	}
	template<class T, class U>
	inline tv3<T> operator /(const tv3<T>& v, const U& s)
	{
		T d = (T)1 / s;
		return (tv3<T>(v.x * d, v.y * d, v.z * d));
	}
	template<class T>
	std::ostream& operator<<(std::ostream& buff, const mst::tv3<T>& v)
	{
		buff << "X: " << v.x << " Y: " << v.y << " Z: " << v.z;
		return buff;
	}
	
	typedef tv3<unsigned char> Color;
}

