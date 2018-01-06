#version 330 core
struct sPointLight {
	vec3 position;	// View space

	// Color properties
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Attenuation properties
	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;
in	vec2 fTexCoord;
in	vec3 FragmentPos;
in	vec3 Normal;

// Uniform variables
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform sPointLight	uPointLight;
uniform vec3 cameraPos;

// Function Prototypes
vec3 CalculatePointLight(sPointLight, vec3);


void main() {
	// At first, define an output color value
	vec3 result = CalculatePointLight(uPointLight, Normal); 

	FragColor = vec4(result, 1.0);
}


// Definition
vec3 CalculatePointLight(sPointLight light, vec3 i_normal) {
	// Attenuation factors
	vec3 lightVec	= light.position - FragmentPos;
	float _D		= sqrt(dot(lightVec, lightVec));
	float _ATT		= 1.0f / (light.constant + (light.linear * _D) + (light.quadratic * pow(_D, 2)));

	// Ambient
	vec3 ambient	= _ATT * light.ambient * vec3(texture(texture_diffuse1, fTexCoord));

	// Diffuse
	vec3 diff_normal= normalize(i_normal);
	vec3 lightDir	= normalize(lightVec);
	float diff		= max(dot(diff_normal, lightDir), 0.0);

	vec3 diffuse	= _ATT * light.diffuse * diff * vec3(texture(texture_diffuse1, fTexCoord));

	// Specular
	vec3 cameraDir	= normalize(cameraPos - FragmentPos);
	vec3 reflectDir = reflect(-lightDir, diff_normal);
	float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), 1.0);

	vec3 spec_tex	= vec3(texture(texture_specular1, fTexCoord));
	vec3 specular	= _ATT * amount * spec_tex * light.specular;
	
	// Result
	return (specular + ambient + diffuse);
}