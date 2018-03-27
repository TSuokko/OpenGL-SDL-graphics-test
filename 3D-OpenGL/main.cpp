#include <GL\glew.h>
#include <SDL\SDL.h>
#include "Vector2.h"
#include "Vector3.h"
#include <fstream>
#include <iostream>

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

	int a;
	std::cin >> a;
	return 0;
}