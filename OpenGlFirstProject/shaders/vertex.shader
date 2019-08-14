#version 440 core

layout (location=0) in  vec3 position;
layout (location=1) in  vec3 color;
uniform mat4 scale;
uniform mat4 rotate;
uniform mat4 translate;
uniform mat4 project;
out vec3 color_out;

void main()
{
	gl_Position =project*translate*rotate*scale*vec4(position, 1.0f);
	color_out = color;
}