#version 330 core
out vec3 Normal;
out vec3 RevisedColor;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;			// Model matrix
uniform mat4 view;			// View matrix
uniform mat4 projection;	// Projection matrix

uniform vec3 lightPos;		// World space coord
uniform vec3 lightColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vec3 Normal = mat3(transpose(inverse(view * model))) * aNormal;
	vec3 FragmentPos = vec3(view * model * vec4(aPos, 1.0));
	vec3 LightPos = vec3(view * vec4(lightPos, 1.0));

	float AMBIENT_STRENGTH = 0.1f;
	float DIST_LIMIT= 20.0f;
	float SPECULAR_LIMIT = 0.5f;
	int	  SPECULAR_INTENSITY = 64;

	// Ambient
	vec3 ambient	= AMBIENT_STRENGTH * lightColor;

	vec3 normal		= normalize(Normal);
	vec3 lightVec	= LightPos - FragmentPos;
	vec3 lightDir	= normalize(lightVec);

	// Diffuse
	float diff		= max(dot(normal, lightDir), 0.0);
	float distance	= min(sqrt(dot(lightVec, lightVec)), DIST_LIMIT);
	vec3 diffuse	= (diff * (DIST_LIMIT - distance) / DIST_LIMIT) * lightColor; 

	// Specular
	vec3 cameraDir	= normalize(-FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec_amt	= pow(max(dot(cameraDir, reflectDir), 0.0), SPECULAR_INTENSITY);
	vec3 specular	= SPECULAR_LIMIT * spec_amt * lightColor;

	RevisedColor = ambient + diffuse + specular;
}