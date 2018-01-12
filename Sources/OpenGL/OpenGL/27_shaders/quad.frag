#version 330 core
struct sPointlight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;	
};

out vec4 FragColor;
in vec3 fFragPos;
in vec2 fTexCoord;
in mat3 fTBN;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;
uniform sampler2D textureNormal;
uniform sPointlight pointlight;
uniform vec3 cameraPos;

vec3 CalculatePointLight(sPointlight light) {
	vec3 color		= vec3(texture(textureDiffuse1, fTexCoord));
	vec3 normal		= texture(textureNormal, fTexCoord).rgb;
	normal			= normalize(normal * 2.0 - 1.0);
	normal			= normalize(fTBN * normal);

	vec3 lightVec	= light.position - fFragPos;

	// Attenuation factors
	float _D		= length(lightVec);
	float _ATT		= 1.0f / (light.constant + (light.linear * _D) + (light.quadratic * pow(_D, 2)));

	// Ambient
	vec3 ambient	= 1.0 * light.ambient * color;

	// Diffuse
	vec3 lightDir	= normalize(lightVec);
	float diff		= max(dot(normal, lightDir), 0.0);

	vec3 diffuse	= _ATT * light.diffuse * diff * color;

	// Specular
	vec3 cameraDir	= normalize(cameraPos - fFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), 32.0);

	vec3 spec_tex	= vec3(texture(textureSpecular1, fTexCoord));
	vec3 specular	= _ATT * amount * spec_tex * light.specular;
	
	// Result
	return (ambient + diffuse + specular);
}

void main() {
	FragColor = vec4(CalculatePointLight(pointlight), 1.0);
}
