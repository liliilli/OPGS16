#version 330 core
// Light properties && struct can't let it out or put them in.
struct uLight {
	vec3 ambient;			// light Colors
	vec3 diffuse;
	vec3 specular;
};

// Object color properties
struct uMaterial {
	sampler2D texture;
	sampler2D specular;
	float intensity;
};

out vec4 FragColor;			// R G B A
in	vec3 Normal;			// in view space
in	vec3 FragmentPos;		// in view space
in	vec3 LightPos;			// in view space (directional light)
in	vec2 TexCoord;			// s t r q

uniform uMaterial	Material;
uniform uLight		Light;

void main() {
	// Ambient
	vec3 ambient	= Light.ambient * texture(Material.texture, TexCoord).rgb;

	// Diffuse
	vec3 normal		= normalize(Normal);
	vec3 lightDir	= normalize(LightPos);
	float diff		= max(dot(normal, lightDir), 0.0);

	vec3 diffuse	= diff * Light.diffuse * vec3(texture(Material.texture, TexCoord));

	// Specular
	vec3 cameraDir	= normalize(-FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), Material.intensity);

	vec3 spec_tex	= vec3(texture(Material.specular, TexCoord));
	vec3 specular	= amount * spec_tex * Light.specular;
	
	// Result
	vec3 result		= ambient + diffuse + specular;
    FragColor		= vec4(result, 1.0); 
}