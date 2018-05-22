#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "control.h"
#include "Cube3D.h"


class OpenGL_Object
{
public:
	OpenGL_Object();
	void init(const char *VertShaderPath, 
		const char *FragShaderPath,
		const char *BMPTexturePath);
	void createObject(const char *ObjectPath, glm::vec3 coordinates);
	void drawObject(GLFWwindow* window);
	void cleanUp();

	~OpenGL_Object();
private:
	GLuint VertexArrayID;
	GLuint programID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint TextureID;
	GLuint LightID;
	GLuint Texture;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	
};

