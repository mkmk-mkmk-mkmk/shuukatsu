#pragma once

#include <math.h>
#include <cmath>


class Vector2
{
public:
	float x, y;
	Vector2() {}
	Vector2(const Vector2& a) :x(a.x), y(a.y) {}
	Vector2(float nx, float ny) :x(nx), y(ny) {}

	Vector2& operator=(const Vector2& a)
	{
		x = a.x; y = a.y;
		return*this;
	}

	bool operator ==(const Vector2& a)const
	{
		return x == a.x && y == a.y;
	}
	bool operator !=(const Vector2& a)const
	{
		return x != a.x || y != a.y;
	}

	void zero() { x = y = 0.0f; }

	Vector2 operator-()const { return Vector2(-x, -y); }

	Vector2 operator+(const Vector2& a)const
	{
		return Vector2(x + a.x, y + a.y);
	}
	Vector2 operator-(const Vector2& a)const
	{
		return Vector2(x - a.x, y - a.y);
	}


	Vector2 operator*(float a)const
	{
		return Vector2(x * a, y * a);
	}

	Vector2 operator /(float a)const
	{
		float oneOverA = 1.0f / a;
		return Vector2(x * oneOverA, y * oneOverA);
	}


	Vector2& operator+=(const Vector2& a)
	{
		x += a.x; y += a.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& a)
	{
		x -= a.x; y -= a.y;
		return *this;
	}
	Vector2& operator*=(float a)
	{
		x *= a; y *= a;
		return *this;
	}
	Vector2& operator /=(float a)
	{
		float oneOverA = 1.0f / a;
		x *= oneOverA; y *= oneOverA;
		return *this;
	}

	void normalize()
	{
		float magSq = x * x + y * y;
		if (magSq > 0.0f)
		{
			float oneOverMag = 1.0f / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
		}
	}

	//float‚Ìâ‘Î’l
	float modulus(float a)
	{
		if (a < 0)
		{
			a = -a;
		}

		return a;
	}

	//Vector2‚Ìâ‘Î’l
	Vector2 modulus(Vector2& a)
	{
		if (a.x < 0)
		{
			a.x = -a.x;
		}
		if (a.y < 0)
		{
			a.y = -a.y;
		}
		return a;
	}

	//float length() const
	//{
	//	return sqrt(x * x + y * y);
	//}

	//‹——£
	float length(Vector2& a)
	{
		return sqrt(a.x * a.x + a.y * a.y);
	}

	//‹——£
	float distance(Vector2& a, Vector2& b)
	{
		Vector2 diff;

		diff.x = a.x - b.x;
		diff.y = a.y - b.y;

		return sqrt(diff.x * diff.x + diff.y * diff.y);
	}

	Vector2 vec(Vector2& a, Vector2& b)
	{
		return Vector2(b.x - a.x, b.y - a.y);
	}
};