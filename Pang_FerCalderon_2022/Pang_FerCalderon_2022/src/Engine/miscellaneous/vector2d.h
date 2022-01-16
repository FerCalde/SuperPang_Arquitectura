
#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#include <math.h>

class vec2
{
public:
	float x;
	float y;

	inline vec2(void) :
		x(0), y(0)
	{ }

	inline vec2(float f)
	{
		x = y = f;
	}

	inline vec2(float _x, float _y)
	{
		x = _x;  y = _y;
	}

	inline vec2(const float f[2])
	{
		x = f[0];  y = f[1];
	}

	inline const float& operator[](int i) const
	{
		return (&x)[i];
	}

	inline float& operator[](int i)
	{
		return (&x)[i];
	}

	inline vec2& operator += (const vec2& v)
	{
		x += v.x;  y += v.y;
		return *this;
	}

	inline vec2& operator -= (const vec2& v)
	{
		x -= v.x;  y -= v.y;
		return *this;
	}

	inline vec2& operator *= (const vec2& v)
	{
		x *= v.x;  y *= v.y;
		return *this;
	}

	inline vec2& operator /= (const vec2& v)
	{
		x /= v.x;  y /= v.y;
		return *this;
	}

	inline vec2& operator *= (float s)
	{
		x *= s;  y *= s;
		return *this;
	}

	inline vec2& operator /= (float s)
	{
		float a = 1.0f / s;
		x *= a;  y *= a;
		return *this;
	}
};

inline vec2 operator + ( const vec2& v1, const vec2& v2 )
{
	return vec2( v1.x+v2.x, v1.y+v2.y );
}

inline vec2 operator - ( const vec2& v1, const vec2& v2 )
{
	return vec2( v1.x-v2.x, v1.y-v2.y );
}

inline vec2 operator * ( const vec2& v1, const vec2& v2 )
{
	return vec2( v1.x*v2.x, v1.y*v2.y );
}

inline vec2 operator / ( const vec2& v1, const vec2& v2 )
{
	return vec2( v1.x/v2.x, v1.y/v2.y );
}

inline vec2 operator * ( const vec2& v, float s )
{
	return vec2( s*v.x, s*v.y );
}

inline vec2 operator / ( const vec2& v, float s )
{
	float a = 1.0f/s;
	return vec2( v.x*a, v.y*a );
}

inline bool operator == ( const vec2& v1, const vec2& v2 )
{
	return  v1.x == v2.x  &&  v1.y == v2.y;
}

inline bool operator != ( const vec2& v1, const vec2& v2 )
{
	return v1.x != v2.x  ||  v1.y != v2.y;
}

inline float vdot( const vec2& v1, const vec2& v2 )
{
	return v1.x*v2.x + v1.y*v2.y;
}

inline float vlen(const vec2& v)
{
	double dValueX = static_cast<double>(v.x);
	double dValueY = static_cast<double>(v.y);
	return static_cast<float>(sqrt((dValueX * dValueX) + (dValueY * dValueY)));
}

inline float vlen2(const vec2& v)
{
	return v.x * v.x + v.y * v.y;
}

inline vec2 vnorm(const vec2& v)
{
	float M = vlen(v);
	return (M == 0.0) ? v : v * (1.0f / M);
}

// Angle between [0,PI].
inline double vang( const vec2& v1, const vec2& v2 )
{
	vec2 v1Norm = vnorm(v1);
	vec2 v2Norm = vnorm(v2);
	float dot = vdot(v1Norm, v2Norm);
	dot = dot > 1.0f ? 1.0f : dot;
	dot = dot < -1.0f ? -1.0f : dot;
	return acos(dot);
}

#endif
