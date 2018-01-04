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

void main() {    
	FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
