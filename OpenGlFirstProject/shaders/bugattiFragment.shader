#version 440 core

out vec4 frag_color;
in vec3 normal_in;
in vec3 toLightPosition;

void main()
{        
	    float diff = max(dot(normal_in, toLightPosition), 0.0f);

		frag_color = vec4(1.0f,1.0f, 1.0f,1.0f)*diff;
}