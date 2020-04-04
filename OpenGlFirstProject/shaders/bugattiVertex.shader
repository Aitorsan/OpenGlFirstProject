#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoords;
layout(location = 2) in vec3 normals;

out vec3 normal_in;
out vec3 toLightPosition;
uniform mat4 camera;
uniform mat4 project;
uniform mat4 model;
uniform vec3 lightPosition;

void main()
{
	gl_Position = project * camera *  model* vec4(position, 1.0f);
	
    normal_in = mat3(transpose(inverse(model )))* normals;
	toLightPosition = normalize(lightPosition - (model * vec4(position, 1.0f)).xyz);
}