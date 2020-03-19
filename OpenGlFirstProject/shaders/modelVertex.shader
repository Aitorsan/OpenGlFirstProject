#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoords;
layout(location = 2) in vec3 normals;

out vec3 color_in;
uniform mat4 camera;
uniform mat4 project;
uniform mat4 model;

void main()
{
	gl_Position = project * camera *model* vec4(position, 1.0f);
	color_in = normals;
}