#pragma once
#include <math.h>
#define PI 3.14159265
class Vector4
{
public:
	Vector4(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	};
	~Vector4() {};
	float getLength()
	{
		length = sqrt(x*x + y*y + z*z + w*w);
		return length;
	}
	Vector4 operator+(Vector4 &ob)
	{
		Vector4 v3Add(x + ob.x, y + ob.y, z + ob.z, w + ob.w);
		return v3Add;
	}
	Vector4 operator-(Vector4 &ob)
	{
		Vector4 v3Sub(x - ob.x, y - ob.y, z - ob.z, w - ob.w);
		return v3Sub;
	}
	Vector4 operator*(float ob)
	{
		Vector4 v3Scalar(x * ob, y * ob, z*ob, w * ob);
		return v3Scalar;
	}

	float length;
	float x;
	float y;
	float z;
	float w;
};

float Vec4_Dot(Vector4 v1, Vector4 v2)
{
	float vDot = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return vDot;
}

float Vec4_Angle(Vector4 v1, Vector4 v2)
{
	float vAngle = acos(Vec4_Dot(v1, v2) / (v1.getLength()*v2.getLength())) * 180 / PI;
	return vAngle;
}

