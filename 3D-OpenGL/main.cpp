#include <GL\glew.h>
#include <SDL\SDL.h>
#include "Vector2.h"
#include "Vector3.h"
#include <fstream>
#include <iostream>
#include "Matrix.h"


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


	Vector4 vector1(1,4,7,2);
	Vector4 vector2(5,3,2,5);
	Vector4 vector3(1,2,3,4);
	Vector4 vector4(4,3,2,1);

	Matrix4 mat1 = Matrix4(vector1, vector2, vector3, vector4);

	mat1.printMat4();
	mat1.inverse().printMat4();
	//std::cout << "Transposed original";
	//mat1.transpose().printMat4();
	//std::cout << "Transposed" << std::endl;
	//printMat4(mat1.transpose());

	/*mat1.printMat4();
	std::cout << "Determinant: "<<mat1.det4x4();
	std::cout << "\nCofactor: " << std::endl;
	mat1.cofactor().transpose().printMat4();
	*/

	//mat1.inverse().printMat4();

	(mat1*mat1.inverse()).printMat4();;

	int a;
	std::cin >> a;
	return 0;
}