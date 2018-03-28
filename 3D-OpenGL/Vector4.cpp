#include "Vector4.h"

float Vector4::getLength()
{
	length = sqrt(x*x + y*y + z*z + w*w);
	return length;
}

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