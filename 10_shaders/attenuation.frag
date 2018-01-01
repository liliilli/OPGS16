#version 330 core

// Light properties && struct can't let it out or put them in.
struct uLight {
	vec3 ambient;			// light Colors
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

// Object color properties
struct uMaterial {
	sampler2D texture;
	sampler2D specular;
	float intensity;
};

out vec4 FragColor;
in	vec3 Normal;			// in view space
in	vec3 FragmentPos;		// in view space
in	vec3 LightPos;			// in view space
in	vec2 TexCoord;

uniform uMaterial	Material;
uniform uLight		Light;

void main() {
	vec3 lightVec	= LightPos - FragmentPos;
	float _D = sqrt(dot(lightVec, lightVec));
	float _ATT = 1.0f / (Light.constant + (Light.linear * _D) + (Light.quadratic * pow(_D, 2)));

	// Ambient
	vec3 ambient	= Light.ambient * vec3(texture(Material.texture, TexCoord));

	// Diffuse
	vec3 normal		= normalize(Normal);
	vec3 lightDir	= normalize(lightVec);
	float diff		= max(dot(normal, lightDir), 0.0);

	vec3 diffuse	= _ATT * Light.diffuse * diff * vec3(texture(Material.texture, TexCoord));

	// Specular
	vec3 cameraDir	= normalize(-FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), Material.intensity);

	vec3 spec_tex	= vec3(texture(Material.specular, TexCoord));
	vec3 specular	= _ATT * amount * spec_tex * Light.specular;
	
	// Result
	vec3 result		= specular + ambient + diffuse;
    FragColor		= vec4(result, 1.0); 
}