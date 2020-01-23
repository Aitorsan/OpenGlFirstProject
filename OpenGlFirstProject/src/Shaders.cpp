#include "include/Shaders.h"
#include "include/Diagnostics.h"
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>

unsigned int CreateShaderProgram(const char * vertexShaderSourcePath, const char * fragmentShaderSourcePath)
{
	auto[VertexID, FragmentID] = GetShadersID(vertexShaderSourcePath, fragmentShaderSourcePath) ;
	unsigned int ShaderProgram = glCreateProgram();
	
	glAttachShader(ShaderProgram, VertexID);
	glAttachShader(ShaderProgram, FragmentID);
	
	glLinkProgram(ShaderProgram);
	
	glDetachShader(ShaderProgram, VertexID);
	glDetachShader(ShaderProgram, FragmentID);
	
	glDeleteShader(VertexID);
	glDeleteShader(FragmentID);
	
	return ShaderProgram;
}

std::tuple<unsigned int , unsigned int > GetShadersID(const char * vetexShaderSourcePath, const char * fragmentShaderSourcePath)
{
	unsigned int VertexID{ };
	unsigned int FragmentID{ };
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
		
		VertexID = glCreateShader(GL_VERTEX_SHADER);
		FragmentID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(VertexID, 1, &vertexSourceCode, nullptr);
		glShaderSource(FragmentID, 1, &fragmentSourceCode, nullptr);
		
		glCompileShader(VertexID);
		DebugShaderCompileStatus(VertexID, "vertex");
		
		glCompileShader(FragmentID);
		DebugShaderCompileStatus(FragmentID, "fragment");   
	}
	return { VertexID,FragmentID };
}

