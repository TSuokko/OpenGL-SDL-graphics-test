#include "Vector3.h"

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