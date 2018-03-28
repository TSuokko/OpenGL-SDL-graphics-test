#include <GL\glew.h>
#include <SDL\SDL.h>
#include "Vector2.h"
#include "Vector3.h"
#include <fstream>
#include <iostream>
#include "Matrix.h"

void printMat4(const Matrix4& m)
{
	printf("\nMatrix 4x4: \n");
	printf("|%2.0f %2.0f %2.0f %2.0f|\n", m.vec1.x, m.vec1.y, m.vec1.z, m.vec1.w);
	printf("|%2.0f %2.0f %2.0f %2.0f|\n", m.vec2.x, m.vec2.y, m.vec2.z, m.vec2.w);
	printf("|%2.0f %2.0f %2.0f %2.0f|\n", m.vec3.x, m.vec3.y, m.vec3.z, m.vec3.w);
	printf("|%2.0f %2.0f %2.0f %2.0f|\n", m.vec4.x, m.vec4.y, m.vec4.z, m.vec4.w);

}
void printVec4(const char* const name, const Vector4& v)
{
	printf("%-12s = <%2.2f,%2.2f,%2.2f,%2.2f>\n", name, v.x, v.y, v.z, v.w);
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
	Vector3 v1(1, 2, 3);
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


	Vector4 vector1(1, 2, 3, 4);
	Vector4 vector2(4, 3, 3, 3);
	Vector4 vector3(1, 2, 2 ,3);
	Vector4 vector4(4, 4, 3 ,2);

	Matrix4 mat1 = Matrix4(vector1, vector2, vector3, vector4);
	printMat4(mat1);

	//std::cout << "Transposed" << std::endl;
	//printMat4(mat1.transpose());


	std::cout << "Determinant: "<<mat1.det4x4();
	std::cout << "\nCofactor: " << std::endl;
	printMat4(mat1.cofactor());


	int a;
	std::cin >> a;
	return 0;
}