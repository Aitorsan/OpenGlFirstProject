#version 440 core

out vec4 frag_color;
in vec3 color_in;
void main()
{
	frag_color = vec4(color_in*0.3f, 1.0f);
}