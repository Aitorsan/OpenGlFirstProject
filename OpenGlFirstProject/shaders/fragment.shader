#version 440 core

in vec3 color_out;
out vec4 fragment_color;

void main()
{
	fragment_color = vec4(color_out,1.0f);
	
}


