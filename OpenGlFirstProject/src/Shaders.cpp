#include "headers/Shaders.h"
#include "headers/Diagnostics.h"
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>

unsigned int CreateShaderProgram(const char * vertexShaderSourcePath, const char * fragmentShaderSourcePath)
{
	Shaders shaders { GetShadersID(vertexShaderSourcePath, fragmentShaderSourcePath) };
	unsigned int ShaderProgram = glCreateProgram();
	
	glAttachShader(ShaderProgram, shaders.VertexID);
	glAttachShader(ShaderProgram, shaders.FragmentID);
	
	glLinkProgram(ShaderProgram);
	
	glDetachShader(ShaderProgram, shaders.VertexID);
	glDetachShader(ShaderProgram, shaders.FragmentID);
	
	glDeleteShader(shaders.VertexID);
	glDeleteShader(shaders.FragmentID);
	
	return ShaderProgram;
}

Shaders GetShadersID(const char * vetexShaderSourcePath, const char * fragmentShaderSourcePath)
{
	Shaders shaderIds { 0,0 };
	std::ifstream vertexShaderFile(vetexShaderSourcePath);
	std::ifstream fragmentShaderFile(fragmentShaderSourcePath);

	if (vertexShaderFile.is_open() && fragmentShaderFile.is_open())
	{
		std::string vsc { std::istreambuf_iterator<char>(vertexShaderFile), std::istreambuf_iterator<char>() };
		std::string fsc { std::istreambuf_iterator<char>(fragmentShaderFile),std::istreambuf_iterator<char>()};
	
		vertexShaderFile.close();
		fragmentShaderFile.close();
		
		const char* vertexSourceCode = vsc.c_str();
		const char* fragmentSourceCode = fsc.c_str();
		
		shaderIds.VertexID = glCreateShader(GL_VERTEX_SHADER);
		shaderIds.FragmentID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(shaderIds.VertexID, 1, &vertexSourceCode, nullptr);
		glShaderSource(shaderIds.FragmentID, 1, &fragmentSourceCode, nullptr);
		
		glCompileShader(shaderIds.VertexID);
		DebugShaderCompileStatus(shaderIds.VertexID, "vertex");
		
		glCompileShader(shaderIds.FragmentID);
		DebugShaderCompileStatus(shaderIds.FragmentID, "fragment");   
	}
	return shaderIds;
}

