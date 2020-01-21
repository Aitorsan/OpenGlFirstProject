#version 440 core

in vec3 TexCoords;
out vec4 FragColor;

layout(binding = 0) uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, TexCoords);
}
