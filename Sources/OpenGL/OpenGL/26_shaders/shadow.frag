#version 330 core
struct sPointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} gs_in;

uniform sampler2D	textureDiffuse1;
uniform sampler2D	textureSpecular1;
uniform samplerCube shadowMap;

uniform sPointLight pointlight;
uniform vec3		cameraPos;

uniform float		farPlane;

float ShadowCalculation(vec3 fragPos) {
	vec3 frag_to_light	= fragPos - pointlight.position;

	float current_depth = length(frag_to_light);
	
	float shadow = 0.0f;
	float bias = max(0.25 * (1.0 - dot(normalize(-frag_to_light), normalize(gs_in.Normal))), 0.05);
	
	float amnt = 1 / 48.f;
	for (int y = -1; y <= 1; ++y) {
		for (int x = -1; x <= 1; ++x) {
			for (int z = -1; z <= 1; ++z) {
				vec3 offset = vec3(x * amnt, y * amnt, z * amnt);
				float closest_depth = texture(shadowMap, frag_to_light + offset).r * farPlane;
				
				if (current_depth - bias > closest_depth)
					shadow += 1.0f;
			}
		}
	}

	shadow /= 32.0f;
	return shadow;
}

vec3 CalculatePointLight(sPointLight light) {
	// Attenuation factors
	vec3 lightVec	= light.position - gs_in.FragPos;
	float _D		= length(lightVec);
	float _ATT		= 1.0f / (light.constant + (light.linear * _D) + (light.quadratic * pow(_D, 2)));

	vec3 color		= vec3(texture(textureDiffuse1, gs_in.TexCoord));

	// Ambient
	vec3 ambient	= _ATT * light.ambient * color;

	// Diffuse
	vec3 normal		= gs_in.Normal;
	vec3 lightDir	= normalize(lightVec);
	float diff		= max(dot(normal, lightDir), 0.0);

	vec3 diffuse	= _ATT * light.diffuse * diff * color;

	// Specular
	vec3 cameraDir	= normalize(cameraPos - gs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float amount	= pow(max(dot(cameraDir, reflectDir), 0.0), 32.0);

	vec3 spec_tex	= vec3(texture(textureSpecular1, gs_in.TexCoord));
	vec3 specular	= _ATT * amount * spec_tex * light.specular;
	
	// Result
	float shadow	= ShadowCalculation(gs_in.FragPos);
	return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main() {
	FragColor = vec4(CalculatePointLight(pointlight), 1.0);
}
