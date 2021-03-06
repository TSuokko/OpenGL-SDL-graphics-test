#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
class Matrix4
{
public:
	Matrix4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4)
	{
		vec1 = v1;
		vec2 = v2;
		vec3 = v3;
		vec4 = v4;

	};
	~Matrix4();

	void printMat4(const char* const name);
	float det4x4();
	float dotMat4(Vector4 row, Vector4 column);
	Matrix4 transpose();
	Matrix4 cofactor();

	Matrix4 scaling(float s);

	Matrix4 inverse();
	
	Matrix4 operator*(const Matrix4 &o);
	
	
	Vector4 Matrix4::operator*(const Vector4& o);

	
	Matrix4 translation(const Vector3& t);
	 

	Vector4 vec1, vec2, vec3, vec4;
	
};

class Matrix3
{
public:
	Matrix3(Vector3 v1, Vector3 v2, Vector3 v3)
	{
		vec1 = v1;
		vec2 = v2;
		vec3 = v3;
	};

	~Matrix3() {};

	Vector3 vec1, vec2, vec3;
	
};

extern float det3x3(Vector3 vec1, Vector3 vec2, Vector3 vec3);
extern float det2x2(Vector2 vec1, Vector2 vec2);
extern Matrix4 rotationX(float a);
extern Matrix4 rotationY(float a);
extern Matrix4 rotationZ(float a);

class Matrix2
{
public:
	Matrix2(Vector2 v1, Vector2 v2)
	{
		vec1 = v1;
		vec2 = v2;
	};
	~Matrix2() {};

	Vector2 vec1;
	Vector2 vec2;
	

};



