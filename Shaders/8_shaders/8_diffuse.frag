#version 330 core
out vec4 FragColor;
in	vec3 Normal;
in	vec3 FragmentPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;

void main() {
	float AMBIENT_STRENGTH = 0.1f;
	float DIST_LIMIT= 10.0f;

	vec3 ambient	= AMBIENT_STRENGTH * lightColor;

	vec3 normal		= normalize(Normal);
	vec3 lightVec	= lightPos - FragmentPos;
	vec3 lightDir	= normalize(lightVec);

	float diff		= max(dot(normal, lightDir), 0.0);

	float distance	= min(sqrt(dot(lightVec, lightVec)), DIST_LIMIT);
	vec3 diffuse	= (diff * (DIST_LIMIT - distance) / DIST_LIMIT) * lightColor; 

	vec3 result		= (ambient + diffuse) * objectColor;
    FragColor		= vec4(result, 1.0); 
}