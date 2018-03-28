#include "Matrix.h"

Matrix4::~Matrix4()
{
}

/*
| a b c d |
| e f g h |
| i j k l |
| m n o p |
*/
float Matrix4::det4x4()
{
	float determinant4x4 =

		  vec1.x * (det3x3(Vector3(vec2.y, vec2.z, vec2.w), Vector3(vec3.y, vec3.z, vec3.w), Vector3(vec4.y, vec4.z, vec4.w)))
		- vec1.y * (det3x3(Vector3(vec2.x, vec2.z, vec2.w), Vector3(vec3.x, vec3.z, vec3.w), Vector3(vec4.x, vec4.z, vec4.w)))
		+ vec1.z * (det3x3(Vector3(vec2.x, vec2.y, vec2.w), Vector3(vec3.x, vec3.y, vec3.w), Vector3(vec4.x, vec4.y, vec4.w)))
		- vec1.w * (det3x3(Vector3(vec2.x, vec2.y, vec2.z), Vector3(vec3.x, vec3.y, vec3.z), Vector3(vec4.x, vec4.y, vec4.z)));

	return determinant4x4;
}

/*
|m11 m12 m13|
|m21 m22 m23|
|m31 m32 m33|
*/
float det3x3(Vector3 vec1, Vector3 vec2, Vector3 vec3)
{

	float determinant3x3 =

		  vec1.x * (det2x2(	Vector2(vec2.y, vec2.z),
							Vector2(vec3.y, vec3.z))) 

		- vec1.y * (det2x2(	Vector2(vec2.x, vec2.z),
							Vector2(vec3.x, vec3.z))) 

		+ vec1.z * (det2x2(	Vector2(vec2.x, vec2.y),
							Vector2(vec3.x, vec3.y)));
		
	return determinant3x3;
}

/*
|m11 m12|
|m21 m22|
*/
float det2x2(Vector2 vec1, Vector2 vec2)
{
	float determinant2x2 = (vec1.x * vec2.y) - (vec2.x * vec1.y);
	return determinant2x2;
}

/*Matrix4 Matrix4::transpose()
{
	Matrix4 transposed = 
	Matrix4
	(
		Vector4(a, e, i, m), 
		Vector4(b, f, j, n), 
		Vector4(c, g, k, o), 
		Vector4(d, h, l, p)
	);
	return transposed;
}*/


/*

|a b c d|
|e f g h|
|i j k l|
|m n o p|

*/
Matrix4 Matrix4::cofactor()
{
	Matrix4 cofactor =
		Matrix4
		(
			Vector4(
				 det3x3(Vector3(vec2.y, vec2.z, vec2.w), Vector3(vec3.y, vec3.z, vec3.w), Vector3(vec4.y, vec4.z, vec4.w)), //c11
				-det3x3(Vector3(vec2.x, vec2.z, vec2.w), Vector3(vec3.x, vec3.z, vec3.w), Vector3(vec4.x, vec4.z, vec4.w)), //c12
				 det3x3(Vector3(vec2.x, vec2.y, vec2.w), Vector3(vec3.x, vec3.y, vec3.w), Vector3(vec4.x, vec4.y, vec4.w)), //c13
				-det3x3(Vector3(vec2.x, vec2.y, vec2.z), Vector3(vec3.x, vec3.y, vec3.z), Vector3(vec4.x, vec4.y, vec4.z))),//c14
			Vector4(
				-det3x3(Vector3(vec1.y, vec1.z, vec1.w), Vector3(vec3.y, vec3.z, vec3.w), Vector3(vec4.y, vec4.z, vec4.w)), //c21
				 det3x3(Vector3(vec1.x, vec1.z, vec1.w), Vector3(vec3.x, vec3.z, vec3.w), Vector3(vec4.x, vec4.z, vec4.w)), //c22
				-det3x3(Vector3(vec1.x, vec1.y, vec1.w), Vector3(vec3.x, vec3.y, vec3.w), Vector3(vec4.x, vec4.y, vec4.w)), //c23
				 det3x3(Vector3(vec1.x, vec1.y, vec1.z), Vector3(vec3.x, vec3.y, vec3.z), Vector3(vec4.x, vec4.y, vec4.z))),//c24
			Vector4(
				 det3x3(Vector3(vec1.y, vec1.z, vec1.w), Vector3(vec2.y, vec2.z, vec2.w), Vector3(vec4.y, vec4.z, vec4.w)), //c31
				-det3x3(Vector3(vec1.x, vec1.z, vec1.w), Vector3(vec2.x, vec2.z, vec2.w), Vector3(vec4.x, vec4.z, vec4.w)), //c32
				 det3x3(Vector3(vec1.x, vec1.y, vec1.w), Vector3(vec2.x, vec2.y, vec2.w), Vector3(vec4.x, vec4.y, vec4.w)), //c33
				-det3x3(Vector3(vec1.x, vec1.y, vec1.z), Vector3(vec2.x, vec2.y, vec2.z), Vector3(vec4.x, vec4.y, vec4.z))),//c34
			Vector4(
				-det3x3(Vector3(vec1.y, vec1.z, vec1.w), Vector3(vec2.y, vec2.z, vec2.w), Vector3(vec3.y, vec3.z, vec3.w)), //c41
				 det3x3(Vector3(vec1.x, vec1.z, vec1.w), Vector3(vec2.x, vec2.z, vec2.w), Vector3(vec3.x, vec3.z, vec3.w)), //c42
				-det3x3(Vector3(vec1.x, vec1.y, vec1.w), Vector3(vec2.x, vec2.y, vec2.w), Vector3(vec3.x, vec3.y, vec3.w)), //c43
				 det3x3(Vector3(vec1.x, vec1.y, vec1.z), Vector3(vec2.x, vec2.y, vec2.z), Vector3(vec3.x, vec3.y, vec3.z))));
	return cofactor;
}

/*Matrix4 Matrix4::inverse()
{
	Matrix4 inversed =
	Matrix4
	(

	)
	

}

Matrix4 Matrix4::Matrix4::operator*(const Matrix4& o)const
{

}

Vector4 Matrix4::Matrix4::operator*(const Vector4& o)const
{

}

Matrix4 Matrix4::scaling(float s)
{

}

Matrix4 Matrix4::translation(const Vector3& t)
{

}



Matrix4 Matrix4::rotationX(float a)
{

}
Matrix4 Matrix4::rotationY(float a)
{

}
Matrix4 Matrix4::rotationZ(float a)
{

}*/

