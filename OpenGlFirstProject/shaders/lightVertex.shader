#version 440 core

layout(location = 0) in vec3 position;
uniform mat4 camera;
uniform mat4 project;
uniform mat4 model;

void main()
{
	gl_Position = project * camera *model* vec4(position,1.0f);
}