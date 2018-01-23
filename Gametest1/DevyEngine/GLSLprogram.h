#pragma once
#include <GL/glew.h>
#include <string>
class GLSLprogram
{
public:
	GLSLprogram();
	~GLSLprogram();
	void compileShaders(const std::string& vertShaderPath, const std::string& fragShaderPath);
	void linkShaders();
	void addAttribute(const std::string& attributeName);
	void use();
	void unuse();

	GLint getUniformLocation(const std::string& uniformName);
private:

	void compileShader(const std::string& filePath, GLuint& id);

	
	GLuint	_programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
	int _numAttributes;

};

