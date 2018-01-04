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


out vec4 FragColor;
in	vec2 TexCoord;
in	vec3 FragmentPos;
in	vec3 Normal;


// Uniform variables
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform sDirectionalLight	uDirLight;
uniform sPointLight			uPointLight[4];
uniform sSpotlight			uSpotlight;


// Function Prototypes
vec3 CalculateDirectionalLight(sDirectionalLight light, vec3 normal);
vec3 CalculatePointLight(sPointLight, vec3);
vec3 CalculateSpotLight(sSpotlight light, vec3 normal);


void main() {    
	// At first, define an output color value
	vec3 result = vec3(0.0);

	// Directional Light
	result += CalculateDirectionalLight(uDirLight, Normal);

	// Point Light
	for (int i = 0; i < 0; i++)
		result += CalculatePointLight(uPointLight[i], Normal);

	// Spotlight
	result += CalculateSpotLight(uSpotlight, Normal);

	FragColor = vec4(result, 1.0);
}


// Definition
vec3 CalculateDirectionalLight(sDirectionalLight light, vec3 normal) {
	// Ambient
	vec3 ambient		= light.ambient * vec3(texture(texture_diffuse1, TexCoord));

	// Diffuse
	vec3 diff_normal	= normalize(-normal);
	vec3 lightDir		= normalize(light.direction);
	float diff_factor	= max(dot(diff_normal, lightDir), 0.0);

	vec3 diffuse		= diff_factor * light.diffuse * vec3(texture(texture_diffuse1, TexCoord));

	// Specular
	vec3 reflectDir		= reflect(-lightDir, diff_normal);
	vec3 cameraDir		= normalize(-FragmentPos);
	float spec_factor	= pow(max(dot(cameraDir, reflectDir), 0.0), 32.0);

	vec3 spec_tex		= vec3(texture(texture_specular1, TexCoord));
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
	vec3 ambient	= _ATT * light.ambient * vec3(texture(texture_diffuse1, TexCoord));

	// Diffuse
	vec3 diff_normal= normalize(i_normal);
	vec3 lightDir	= normalize(lightVec);
	float diff		= max(dot(diff_normal, lightDir), 0.0);

	vec3 diffuse	= _ATT * light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));

	// Specular
	vec3 cameraDir	= normalize(-FragmentPos);
	vec3 reflectDir = reflect(-lightDir, diff_normal);
	float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), 32.0);

	vec3 spec_tex	= vec3(texture(texture_specular1, TexCoord));
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

		diffuse	= _ATT * light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));

		// Specular
		vec3 cameraDir	= normalize(-FragmentPos);
		vec3 reflectDir = reflect(-lightDir, diff_normal);
		float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), 32.0);

		vec3 spec_tex	= vec3(texture(texture_specular1, TexCoord));
		specular		= _ATT * amount * spec_tex * light.specular;

		// Interpolation
		float epsilon	= light.inCutoff - light.outCutoff;
		float smoothy	= clamp((theta - light.outCutoff) / epsilon, 0.0, 1.0);

		diffuse			*= smoothy;
		specular		*= smoothy;
	}

	// Ambient
	vec3 ambient		= _ATT * light.ambient * vec3(texture(texture_diffuse1, TexCoord));

	// Result
	return (specular + ambient + diffuse);
}
