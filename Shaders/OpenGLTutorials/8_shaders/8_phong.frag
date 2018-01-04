#version 330 core
out vec4 FragColor;
in	vec3 Normal;			// in view space
in	vec3 FragmentPos;		// in view space
in  vec3 LightPos;			// in view space

uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
	float AMBIENT_STRENGTH = 0.1f;
	float DIST_LIMIT= 20.0f;
	float SPECULAR_LIMIT = 0.5f;
	int	  SPECULAR_INTENSITY = 64;

	// Ambient
	vec3 ambient	= AMBIENT_STRENGTH * lightColor;

	vec3 normal		= normalize(Normal);
	vec3 lightVec	= LightPos - FragmentPos;
	vec3 lightDir	= normalize(lightVec);

	// Diffuse
	float diff		= max(dot(normal, lightDir), 0.0);
	float distance	= min(sqrt(dot(lightVec, lightVec)), DIST_LIMIT);
	vec3 diffuse	= (diff * (DIST_LIMIT - distance) / DIST_LIMIT) * lightColor; 

	// Specular
	vec3 cameraDir	= normalize(-FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec_amt	= pow(max(dot(cameraDir, reflectDir), 0.0), SPECULAR_INTENSITY);
	vec3 specular	= SPECULAR_LIMIT * spec_amt * lightColor;

	vec3 result		= (specular + ambient + diffuse) * objectColor;
    FragColor		= vec4(result, 1.0); 
}