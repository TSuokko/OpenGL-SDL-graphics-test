#pragma once
#include <math.h>
#define PI 3.14159265
class Vector3
{
public:
	Vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	};
	Vector3() {};
	~Vector3() {};
	float getLength()
	{
		length = sqrt(x*x + y*y + z*z);
		return length;
	}
	Vector3 operator+(Vector3 &ob)
	{
		Vector3 v3Add(x + ob.x, y + ob.y, z + ob.z);
		return v3Add;
	}
	Vector3 operator-(Vector3 &ob)
	{
		Vector3 v3Sub(x - ob.x, y - ob.y, z - ob.z);
		return v3Sub;
	}
	Vector3 operator*(float ob)
	{
		Vector3 v3Scalar(x * ob, y * ob, z*ob);
		return v3Scalar;
	}

	float length;
	float x;
	float y;
	float z;
};

extern Vector3 cross(Vector3 v1, Vector3 v2);
extern float Vec3_Dot(Vector3 v1, Vector3 v2);
extern float Vec3_Angle(Vector3 v1, Vector3 v2);