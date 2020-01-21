#version 440 core
in vec3 object_position;
in vec2 aText;
in vec3 normal_vector;
out vec4 fragment_color;

uniform sampler2D final_texture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{   
	//ambient 
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * lightColor;

	//diffuse
	vec3 norm = normalize(normal_vector);
	vec3 lightDir = normalize( lightPos - object_position  );
	float diff = max(dot( norm,lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - object_position );
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f),64);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) ;
	fragment_color = texture(final_texture, aText)* vec4( result,1.0f);
}


