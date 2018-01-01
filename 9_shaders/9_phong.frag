#version 330 core
// Light properties && struct can't let it out or put them in.
struct uLight {
	vec3 ambient;			// light Colors
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;
in	vec3 Normal;			// in view space
in	vec3 FragmentPos;		// in view space
in	vec3 LightPos;			// in view space

// Object color properties
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float intensity;
};

uniform Material uMaterial;
uniform uLight Light;

void main() {
	float DIST_LIMIT= 20.0f;
	float SPECULAR_LIMIT = 0.5f;

	// Ambient
	vec3 ambient	= Light.ambient * uMaterial.ambient;

	// Diffuse
	vec3 normal		= normalize(Normal);
	vec3 lightVec	= LightPos - FragmentPos;
	vec3 lightDir	= normalize(lightVec);
	float diff		= max(dot(normal, lightDir), 0.0);

	float distance	= min(sqrt(dot(lightVec, lightVec)), DIST_LIMIT);

	vec3 diffuse	= (diff * (DIST_LIMIT - distance) / DIST_LIMIT) * uMaterial.diffuse * Light.diffuse; 

	// Specular
	vec3 cameraDir	= normalize(-FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec_amt	= pow(max(dot(cameraDir, reflectDir), 0.0), uMaterial.intensity);

	vec3 specular	= SPECULAR_LIMIT * spec_amt * uMaterial.specular * Light.specular;

	// Result
	vec3 result		= specular + ambient + diffuse;
    FragColor		= vec4(result, 1.0); 
}