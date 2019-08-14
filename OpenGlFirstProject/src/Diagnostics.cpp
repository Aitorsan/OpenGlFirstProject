#include "headers/Diagnostics.h"
#include <GL/glew.h>
#include <iostream>
void DebugProgramLinkError(unsigned int shaderProgramId)
{
	int Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &Result);
	glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (!Result )
	{
		char* ProgramErrorMessage = (char*)malloc((InfoLogLength ) * sizeof(char));
		glGetProgramInfoLog(shaderProgramId, InfoLogLength, NULL, &(*ProgramErrorMessage));
		std::cerr << "[Shader linking Error]: " << ProgramErrorMessage << std::endl;
		free(ProgramErrorMessage);
	}
}

void DebugShaderCompileStatus(unsigned int shaderId, const char* shaderType)
{
	int succed = GL_FALSE;
	int InfoLogLength;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &succed);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (!succed)
	{
		char* ShaderErrorMessage = (char*)malloc((InfoLogLength ) * sizeof(char));
		glGetShaderInfoLog(shaderId, InfoLogLength, NULL, ShaderErrorMessage);
		std::cerr << "[ Compile " << shaderType << " Error]: " << ShaderErrorMessage << std::endl;
		free(ShaderErrorMessage);
	}
}
