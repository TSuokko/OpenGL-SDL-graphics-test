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

Vector3 cross(Vector3 v1, Vector3 v2)
{
	Vector3 v3Cross
	(
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x)
	);
	return v3Cross;
}

float Vec3_Dot(Vector3 v1, Vector3 v2)
{
	float vDot = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return vDot;
}

float Vec3_Angle(Vector3 v1, Vector3 v2)
{
	float vAngle = acos(Vec3_Dot(v1, v2) / (v1.getLength()*v2.getLength())) * 180 / PI;
	return vAngle;
}