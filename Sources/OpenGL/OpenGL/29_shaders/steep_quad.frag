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
	vec3 view_dir			= normalize(cameraPos - fFragPos);
	vec2 texture_dir		= vec2(fModel * vec4(view_dir, 0));

	float num_layers		= mix(32.0, 8.0, abs(dot(vec3(0,0,1), view_dir)));
	float layer_depth_offset= 1.0f / num_layers;

	float layer_depth		= layer_depth_offset;

	vec2 offset		= texture_dir / num_layers * 0.016;
	vec2 tex_pos	= texCoord;

	float current_depth = texture(textureHeight, tex_pos).r;
	while (current_depth > layer_depth) {
		current_depth = texture(textureHeight, tex_pos).r;

		tex_pos += offset;
		layer_depth += layer_depth_offset;
	}

	// POM
	float A = layer_depth - current_depth;
	float B = texture(textureHeight, tex_pos - offset).r - (layer_depth - layer_depth_offset);
	float W = A / (A + B);

	vec2 finalTexCoord = W * (tex_pos - offset) + (1.0 - W) * tex_pos;

	return finalTexCoord;
}

vec3 CalculatePointLight(sPointlight light) {
	vec2 texCoord = ParallaxMapping(fTexCoord);

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