#version 330 core

struct sDirectionalLight {
	vec3 direction;	// View space

	// Color properties
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


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


struct sSpotlight {
	vec3 direction;	// View space
	vec3 position;

	// Color properties
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Cutoff properties
	float inCutoff;
	float outCutoff;

	// Attenuation properties
	float constant;
	float linear;
	float quadratic;
};


struct sMaterial {
	sampler2D texture;
	sampler2D specular;
	float intensity;
};


// Defines
#define NR_POINT_LIGHTS		4


// Out & In
out vec4 FragColor;			// R G B A

in	vec3 Normal;			// in view space
in	vec3 FragmentPos;		// in view space
in	vec2 TexCoord;			// s t r q


// Uniform variables
uniform sDirectionalLight	uDirLight;
uniform sPointLight			uPointLight[NR_POINT_LIGHTS];
uniform sSpotlight			uSpotlight;
uniform sMaterial			uMaterial;


// Function Prototypes
vec3 CalculateDirectionalLight(sDirectionalLight light, vec3 normal);
vec3 CalculatePointLight(sPointLight, vec3);
vec3 CalculateSpotLight(sSpotlight light, vec3 normal);


// Main
void main() {
	// At first, define an output color value
	vec3 result = vec3(0.0);

	// Directional Light
	result += CalculateDirectionalLight(uDirLight, Normal);

	// Point Light
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalculatePointLight(uPointLight[i], Normal);

	// Spotlight
	result += CalculateSpotLight(uSpotlight, Normal);

	FragColor = vec4(result, 1.0);
}


// Definition
vec3 CalculateDirectionalLight(sDirectionalLight light, vec3 normal) {
	// Ambient
	vec3 ambient		= light.ambient * texture(uMaterial.texture, TexCoord).rgb;

	// Diffuse
	vec3 diff_normal	= normalize(-normal);
	vec3 lightDir		= normalize(light.direction);
	float diff_factor	= max(dot(diff_normal, lightDir), 0.0);

	vec3 diffuse		= diff_factor * light.diffuse * vec3(texture(uMaterial.texture, TexCoord));

	// Specular
	vec3 reflectDir		= reflect(-lightDir, diff_normal);
	vec3 cameraDir		= normalize(-FragmentPos);
	float spec_factor	= pow(max(dot(cameraDir, reflectDir), 0.0), uMaterial.intensity);

	vec3 spec_tex		= vec3(texture(uMaterial.specular, TexCoord));
	vec3 specular		= spec_factor * spec_tex * light.specular;
	
	// Result
	return (ambient + diffuse + specular);
}


vec3 CalculatePointLight(sPointLight light, vec3 i_normal) {
	// Attenuation factors
	vec3 lightVec	= light.position - FragmentPos;
	float _D		= sqrt(dot(lightVec, lightVec));
	float _ATT		= 1.0f / (light.constant + (light.linear * _D) + (light.quadratic * pow(_D, 2)));

	// Ambient
	vec3 ambient	= _ATT * light.ambient * vec3(texture(uMaterial.texture, TexCoord));

	// Diffuse
	vec3 diff_normal= normalize(i_normal);
	vec3 lightDir	= normalize(lightVec);
	float diff		= max(dot(diff_normal, lightDir), 0.0);

	vec3 diffuse	= _ATT * light.diffuse * diff * vec3(texture(uMaterial.texture, TexCoord));

	// Specular
	vec3 cameraDir	= normalize(-FragmentPos);
	vec3 reflectDir = reflect(-lightDir, diff_normal);
	float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), uMaterial.intensity);

	vec3 spec_tex	= vec3(texture(uMaterial.specular, TexCoord));
	vec3 specular	= _ATT * amount * spec_tex * light.specular;
	
	// Result
	return (specular + ambient + diffuse);
}


vec3 CalculateSpotLight(sSpotlight light, vec3 normal) {
	vec3 lightVec	= light.position - FragmentPos;
	float _D		= sqrt(dot(lightVec, lightVec));
	float _ATT		= 1.0f / (light.constant + (light.linear * _D) + (light.quadratic * pow(_D, 2)));

	// CutOff
	vec3 lightDir		= normalize(lightVec);
	float theta			= dot(-lightDir, normalize(light.direction));
	
	vec3 diffuse		= vec3(0.0);
	vec3 specular		= vec3(0.0);

	// If theta between fragment position and light source is bigger than Light.cutoff,
	// then calculate diffuse and specular;

	if (theta > light.outCutoff) {
		// Diffuse
		vec3 diff_normal= normalize(normal);
		float diff		= max(dot(diff_normal, lightDir), 0.0);

		diffuse	= _ATT * light.diffuse * diff * vec3(texture(uMaterial.texture, TexCoord));

		// Specular
		vec3 cameraDir	= normalize(-FragmentPos);
		vec3 reflectDir = reflect(-lightDir, diff_normal);
		float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), uMaterial.intensity);

		vec3 spec_tex	= vec3(texture(uMaterial.specular, TexCoord));
		specular		= _ATT * amount * spec_tex * light.specular;

		// Interpolation
		float epsilon	= light.inCutoff - light.outCutoff;
		float smoothy	= clamp((theta - light.outCutoff) / epsilon, 0.0, 1.0);

		diffuse			*= smoothy;
		specular		*= smoothy;
	}

	// Ambient
	vec3 ambient		= _ATT * light.ambient * vec3(texture(uMaterial.texture, TexCoord));

	// Result
	return (specular + ambient + diffuse);
}
