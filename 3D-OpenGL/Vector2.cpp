#include "Vector2.h"

float Vec2_Dot(Vector2 v1, Vector2 v2)
{
	float vDot = v1.x*v2.x + v1.y*v2.y;
	return vDot;
}

float Vec2_Angle(Vector2 v1, Vector2 v2)
{
	float vAngle = acos(Vec2_Dot(v1, v2) / (v1.getLength()*v2.getLength())) * 180 / PI;
	return vAngle;
}