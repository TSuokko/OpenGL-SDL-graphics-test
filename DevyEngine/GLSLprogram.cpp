#include "GLSLprogram.h"
#include "Errors.h"
#include <fstream>
#include <vector>
namespace DevyEngine
{
	//The : _numAttributes(0) ect. is an initialization list. It is a better way to initialize variables, since it avoids an extra copy. 
	GLSLprogram::GLSLprogram() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0), _numAttributes(0)
	{
	}

	GLSLprogram::~GLSLprogram()
	{
	}

	//Compiles the shaders into a form that your GPU can understand
	void GLSLprogram::compileShaders(const std::string& vertShaderPath, const std::string& fragShaderPath)
	{
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.
		//Get a program object.
		_programID = glCreateProgram();
		//Create the vertex shader object, and store its ID
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0)
		{
			fatalError("Vertex shader failed to be created");
		}
		//Create the fragment shader object, and store its ID
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0)
		{
			fatalError("Fragment shader failed to be created");
		}
		//Compile each shader
		compileShader(vertShaderPath, _vertexShaderID);
		compileShader(fragShaderPath, _fragmentShaderID);
	}
	void GLSLprogram::linkShaders()
	{
		//Attach our shaders to our program

		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);
		//Link our program
		glLinkProgram(_programID);
		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;

		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			glDeleteProgram(_programID);
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders  failed to link");
		}

		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	void GLSLprogram::addAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	GLint GLSLprogram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + "not found in shader");
		}
		return location;
	}

	void GLSLprogram::use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}
	void GLSLprogram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLprogram::compileShader(const std::string& filePath, GLuint& id)
	{
		std::ifstream vertexFile(filePath);
		if (vertexFile.fail())
		{
			perror(filePath.c_str());
			fatalError("Failed to open" + filePath);
		}

		std::string fileContents;
		std::string line;
		while (std::getline(vertexFile, line))
		{
			fileContents += line + "\n";
		}

		vertexFile.close();

		const char* contentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &contentsPtr, nullptr);

		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLenght = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLenght);
			std::vector<char>errorLog(maxLenght);
			glGetShaderInfoLog(id, maxLenght, &maxLenght, &errorLog[0]);
			glDeleteShader(id);
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");
			return;
		}
	}
}