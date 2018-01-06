#version 330 core
struct sDirectionalLight {
	vec3 direction;	

	// Color properties
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosInLight;
} gs_in;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;
uniform sampler2D shadowMap;
uniform sDirectionalLight lightDirectional;

uniform vec3 lightPos;
uniform vec3 cameraPos;

float ShadowCalculation(vec4 fragPosInLight, vec3 normal, vec3 lightDir) {
	vec3 proj_coord		= fragPosInLight.xyz / fragPosInLight.w;
	proj_coord			= proj_coord * 0.5f + 0.5f;

	//float closest_depth = texture(shadowMap, proj_coord.xy).r;
	float current_depth = proj_coord.z;
	float shadow = 0.0f;
	
	if (current_depth <= 1.0f) {
		vec2 texel		= 1.0f / textureSize(shadowMap, 0);
		float bias		= max(0.0175 * (1.0 - dot(lightDir, normal)), 0.0025);

		for (int x = -1; x <= 1; ++x) {
			for(int y = -1; y <= 1; ++y) {
				float pcf_depth = texture(shadowMap, proj_coord.xy + vec2(x,y) * texel).r;
				shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
			}
		}

		shadow /= 9.0f;
	}

	return shadow;
}

vec3 CalculateDirectionalLight(sDirectionalLight light) {
	// Ambient
	vec3 color			= vec3(texture(textureDiffuse1, gs_in.TexCoord));
	vec3 ambient		= light.ambient * color;
	vec3 normal			= normalize(gs_in.Normal);


	// Diffuse
	vec3 lightDir		= normalize(-light.direction);
	float fac_diffuse	= max(dot(normal, lightDir), 0.0);

	vec3 diffuse		= fac_diffuse * light.diffuse * color;

	// Specular
	vec3 spec_tex		= vec3(texture(textureSpecular1, gs_in.TexCoord));
	vec3 halfway		= normalize(lightDir + (cameraPos - gs_in.FragPos));
	float fac_specular	= pow(max(dot(halfway, normal), 0.0), 32.0);

	vec3 specular		= fac_specular * spec_tex * light.specular;
	
	// Result
	float shadow		= ShadowCalculation(gs_in.FragPosInLight, normal, lightDir);
	return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main() {
	FragColor = vec4(CalculateDirectionalLight(lightDirectional), 1.0);
}
