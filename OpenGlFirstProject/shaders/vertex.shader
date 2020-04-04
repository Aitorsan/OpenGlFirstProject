#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 NormalVector;
layout (location = 2) in vec2 atexture;
layout (location = 3) in mat4 model;

uniform mat4 camera;
uniform mat4 project;

out vec2 aText;
out vec3 normal_vector;
out vec3 object_position;

void main()
{
	vec4 objectPosition = project*camera*model * vec4(position, 1.0f);
	object_position = objectPosition.xyz;
	gl_Position = objectPosition;
	normal_vector = mat3(transpose(inverse(model)))*NormalVector;
	aText = atexture.xy;
}