#version 440 core

layout(location = 0) in vec3 positions;
uniform mat4 model;
uniform mat4 camera;
uniform mat4 project;
out vec3 TexCoords;
void main()
{   
	vec4 aPos = project * camera * model*vec4(positions, 1.0f);
	TexCoords = positions;
	gl_Position = aPos;
}