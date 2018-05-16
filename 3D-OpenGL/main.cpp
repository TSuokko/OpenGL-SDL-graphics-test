#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OpenGL_Object.h"
// Include GLEW
//#include <GL/glew.h>
// Include GLFW
//#include <GLFW/glfw3.h>
GLFWwindow* window;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

/*#include "shader.h"
#include "control.h"
#include "Cube3D.h"*/
#include <time.h>



/*
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
*/


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Learning OpenGL", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);
	// background color
	glClearColor(0.8f, 0.5f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	


	OpenGL_Object Object1;
	Object1.init("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt", "Textures/uvtemplate1.bmp");
	Object1.createObject("Objects/cube.obj", glm::vec3(1.0, 0.0,-1.0));

	OpenGL_Object Object2;
	Object2.init("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt", "Textures/uvtemplate1.bmp");
	Object2.createObject("Objects/Suzanne.obj", glm::vec3(-3.0, -2.0, 4.0));



	
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Object1.drawObject(window);	
		Object2.drawObject(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	Object1.cleanUp();
	//Object2.cleanUp();
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}