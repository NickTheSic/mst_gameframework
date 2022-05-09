#pragma once
#include <iostream>

namespace mst
{
	template <class T>
	union tv2
	{
		tv2() :x{ 0 }, y{ 0 }{}
		tv2(T _x, T _y) :x{ _x }, y{ _y }{}
		tv2(const tv2<T>& v) :x{ v.x }, y{ v.y }{}
		template<class U>
		tv2(const tv2<U>&v) : x{ (T)v.x }, y{ (T)v.y }{}
		struct { T x, y; };
		struct { T u, v; };
		struct { T w, h; };
		inline T& operator[](const int& i)
		{
			return dat[i];
		}
		inline const T& operator[](const int& i) const
		{
			return dat[i];
		}
		inline tv2<T>& operator*=(const T& s)
		{
			x *= s; y *= s;
			return (*this);
		}
		template<class U>
		inline tv2<T>& operator*=(const U& s)
		{
			x *= s; y *= s;
			return (*this);
		}
		inline tv2<T>& operator/=(const T& s)
		{
			x /= s; y /= s;
			return (*this);
		}
		inline tv2<T>& operator/=(const tv2<T>& v)
		{
			x /= v.x; y /= v.y;
			return (*this);
		}
		inline tv2<T>& operator+=(const tv2<T>& v)
		{
			x += v.x;
			y += v.y;
			return (*this);
		}
		inline tv2<T>& operator-=(const tv2<T>& v)
		{
			x -= v.x;
			y -= v.y;
			return (*this);
		}
		inline tv2<T>& operator=(const tv2<T>& v)
		{
			x = v.x;
			y = v.y;
			return (*this);
		}
		inline bool operator ==(const tv2<T>& v)
		{
			return v.x == x && v.y == y;
		}
		inline bool operator !=(const tv2<T>& v)
		{
			return (v.x != x) && (v.y != y);
		}
	private:
		T dat[2];
	};

	template<class T>
	inline tv2<T> operator*(const tv2<T>& v, const T& s)
	{
		return tv2<T>(v.x * (T)s, v.y * (T)s);
	}
	template<class T, class U>
	inline tv2<T> operator*(const tv2<T>& v, const U& s)
	{
		return tv2<T>(v.x * s, v.y * s);
	}
	template<class T>
	inline tv2<T> operator /(const tv2<T>& v, const T& s)
	{
		return (tv2<T>(v.x / s, v.y / s));
	}
	template<class T, class U>
	inline tv2<T> operator /(const tv2<T>& v, const U& s)
	{
		return (tv2<T>(v.x / s, v.y / s));
	}
	template<class T>
	inline tv2<T> operator -(const tv2<T>& v, const tv2<T>& v2)
	{
		return (tv2<T>(v.x - v2.x, v.y - v2.y));
	}
	template<class T>
	inline tv2<T> operator +(const tv2<T>& v, const tv2<T>& v2)
	{
		return (tv2<T>(v.x + v2.x, v.y + v2.y));
	}
	template<class T, class U>
	inline bool operator ==(const tv2<T>& v, const tv2<U>& v2)
	{
		return v.x == v2.x && v.y == v2.y;
	}
	template<class T, class U>
	inline bool operator !=(const tv2<T>& v, const tv2<U>& v2)
	{
		return (v.x != v2.x) && (v.y != v2.y);
	}
	template<class T>
	std::ostream& operator<<(std::ostream& buff, const mst::tv2<T>& v)
	{
		buff << "X: " << v.x << " Y: " << v.y;
		return buff;
	}

	typedef tv2<float> v2f; typedef tv2<int> v2i;
}
