#include <GL\glew.h>
#include <SDL\SDL.h>
#include "Vector2.h"
#include "Vector3.h"
#include <fstream>
#include <iostream>
#include "Matrix.h"

float toRadians(float a)
{
	float radians = (a * PI) / 180;
	return radians;
}

void printVec4(const char* const name, const Vector4& v)
{
	printf("\n%-5s = <%2.2f,%2.2f,%2.2f,%2.2f>\n", name, v.x, v.y, v.z, v.w);
}
void printVec3(const char* const name, const Vector3& v)
{
	printf("%-12s = <%2.2f,%2.2f,%2.2f>\n", name, v.x, v.y, v.z);
}
void printVec2(const char* const name, const Vector2& v)
{
	printf("%-12s = <%2.2f,%2.2f>\n", name, v.x, v.y);
}

int main(int argc, char** argv)
{	
	/*Vector3 v1(1, 2, 3);
	Vector3 v2(4, 4, 5);
	printVec3("v1", v1);
	printVec3("v2", v2);
	printVec3("v1+v2", v1 + v2);
	printVec3("v1-v2", v1 - v2);
	printVec3("v1*3", v1*3);
	printVec3("v1 cross v2", cross(v1, v2));
	std::cout << "\nv1 and v2 angle: " << Vec3_Angle(v1, v2) << std::endl;
	
	Vector2 v3(3, 2);
	Vector2 v4(1, 6);
	printVec2("\nv3", v3);
	printVec2("v4", v4);
	std::cout << "\nv3 and v4 angle: "<<Vec2_Angle(v3, v4) << std::endl;

	
	Vector4 vector1(1,5,2,2);
	Vector4 vector2(5,3,2,5);
	Vector4 vector3(1,2,3,4);
	Vector4 vector4(4,1,2,1);

	Matrix4 mat1 = Matrix4(vector1, vector2, vector3, vector4);

	mat1.printMat4("Matrix1");
	mat1.inverse().printMat4("Inversed");

	(mat1*mat1.inverse()).printMat4("original times inverse ");*/

	Matrix4 mat1 = Matrix4(
		Vector4(1,0,0,0),
		Vector4(0,1,0,0),
		Vector4(0,0,1,0),
		Vector4(0,0,0,1));

	Matrix4 rotMatX = rotationX(toRadians(45));
	Matrix4 rotMatY = rotationY(toRadians(180));
	Matrix4 rotMatZ = rotationZ(toRadians(90));

	Vector4 res1 = mat1 * Vector4(1, 2, 3, 4);
	Vector4 res2 = rotMatX  * Vector4(1, 0, 0, 0);
	Vector4 res3 = rotMatY * Vector4(1, 0, 0, 0);
	Vector4 res4 = rotMatZ * Vector4(0, 1, 0, 0);
	Vector4 res5 = rotMatX.inverse() * Vector4(0, 1, 0, 0);

	/*rotMatX.printMat4("rotMatX ");
	rotMatY.printMat4("rotMatY ");
	rotMatZ.printMat4("rotMatZ ");
	
	rotMatX.inverse().printMat4("rotMatX Inverse ");*/

	//Vector4 res6 = rotMatZ *  rotMatY * rotMatX.inverse() * Vector4(-1,-1,-1,0);

	Matrix4 res6 = rotMatX * rotMatX.inverse();

	printVec4("res1", res1);
	printVec4("res2", res2);
	printVec4("res3", res3);
	printVec4("res4", res4);
	printVec4("res5", res5);
	//printVec4("res6", res6);
	res6.printMat4("rotation x * x.i ");
	

	int a;
	std::cin >> a;
	return 0;
}