#version 440 core

out vec4 frag_color;
uniform vec3 ligthColor;
void main()
{
	frag_color = vec4(ligthColor,1.0f);
}