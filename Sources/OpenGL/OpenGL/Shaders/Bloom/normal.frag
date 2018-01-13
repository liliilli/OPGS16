#version 430 core

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

in VS_OUT {
	vec3 fragmentPos;
	vec3 normalDir;
	vec2 textureCoord;
} fs_in;

uniform sPointLight uPointLight[2];
uniform vec3 uCameraPos;

layout (binding = 0) uniform sampler2D uDiffuse;
layout (binding = 1) uniform sampler2D uSpecular;
layout (binding = 2) uniform sampler2D uEmission;

out vec4 finalColor;

vec3 CalculatePointLight(sPointLight light, vec3 i_normal);
void main() {
	vec3 result;
	for (int i = 0; i < uPointLight.length(); ++i) {
		result += CalculatePointLight(uPointLight[i], fs_in.normalDir);
	}

	finalColor = vec4(result, 1.0);
}

vec3 CalculatePointLight(sPointLight light, vec3 i_normal) {
	// Attenuation factors
	vec3 lightVec	= light.position - fs_in.fragmentPos;
	float _D		= sqrt(dot(lightVec, lightVec));
	float _ATT		= 1.0f / (light.constant + (light.linear * _D) + (light.quadratic * pow(_D, 2)));
	vec3 color		= vec3(texture(uDiffuse, fs_in.textureCoord));

	// Ambient
	vec3 ambient	= _ATT * light.ambient * color;

	// Diffuse
	vec3 lightDir	= normalize(lightVec);
	float diff		= max(dot(i_normal, lightDir), 0.0);

	vec3 diffuse	= _ATT * light.diffuse * diff * color;

	// Specular
	vec3 cameraDir	= normalize(uCameraPos - fs_in.fragmentPos);

	vec3 H			= normalize(cameraDir + lightVec);
	float amount	= pow(max(dot(H, i_normal), 0.0), 0.5);

	vec3 spec_tex	= vec3(texture(uSpecular, fs_in.textureCoord));
	vec3 specular	= _ATT * amount * spec_tex * light.specular;
	
	// Result
	return (specular + ambient + diffuse);
}