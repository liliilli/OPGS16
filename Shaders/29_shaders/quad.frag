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
in vec3 fNormal;
in mat3 fTBN;
in mat4 fModel;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;
uniform sampler2D textureNormal;
uniform sampler2D textureHeight;
uniform sPointlight pointlight;
uniform vec3 cameraPos;

vec2 ParallaxMapping(vec2 texCoord) {
	float height	= 0.f;
	float P = 0.005f;

	vec2 xy[4] = { vec2(-P, -P), vec2(P, P), vec2(-P, P), vec2(P, -P) };
	for (int i = 0; i < 4; ++i) {
		height += texture(textureHeight, texCoord + xy[i]).r / 4;
	}

	vec2 height_offset = vec2(fModel * vec4(normalize(cameraPos - fFragPos), 0)) * (1 - height) * 0.1;
	return texCoord - height_offset;
}

vec3 CalculatePointLight(sPointlight light) {
	vec2 texCoord = ParallaxMapping(fTexCoord);
	if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
		discard;

	vec3 lightVec	= light.position - fFragPos;
	vec3 color		= vec3(texture(textureDiffuse1, texCoord));
	vec3 normal		= texture(textureNormal, texCoord).rgb;
	normal			= normalize(normal * 2.0 - 1.0);
	normal			= normalize(fTBN * normal);

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

	vec3 spec_tex	= vec3(texture(textureSpecular1, texCoord));
	vec3 specular	= _ATT * amount * spec_tex * light.specular;
	
	// Result
	return (ambient + diffuse + specular);
}

void main() {
	FragColor = vec4(CalculatePointLight(pointlight), 1.0);
}
