#pragma once
#include <math.h>
#define PI 3.14159265
class Vector2
{
public:
	Vector2(float _x, float _y  ) 
	{
		x = _x;
		y = _y;
	};
	Vector2() {};
	~Vector2() {};
	float getLength()
	{
		length = sqrt(x*x+y*y);
		return length;
	}
	
	Vector2 operator+(Vector2 &ob)
	{
		Vector2 v2Add(x+ob.x, y+ob.y);
		return v2Add;
	}
	Vector2 operator-(Vector2 &ob)
	{
		Vector2 v2Sub(x - ob.x, y - ob.y);
		return v2Sub;
	}
	Vector2 operator*(float ob)
	{
		Vector2 v2Scalar(x * ob, y * ob);
		return v2Scalar;
	}

	float length;
	float x;
	float y;
};

extern float Vec2_Dot(Vector2 v1, Vector2 v2);
extern float Vec2_Angle(Vector2 v1, Vector2 v2);


