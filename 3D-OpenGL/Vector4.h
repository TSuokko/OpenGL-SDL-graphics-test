#pragma once

#ifndef _VEC_4_
#define _VEC_4_

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

	Vector4(){};

	~Vector4() {};
	float getLength();

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

extern float Vec4_Dot(Vector4 v1, Vector4 v2);

extern float Vec4_Angle(Vector4 v1, Vector4 v2);


#endif