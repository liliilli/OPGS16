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

uniform sPointLight uPointLight[4];
uniform vec3 uCameraPos;

layout (binding = 0) uniform sampler2D uDiffuse;
layout (binding = 1) uniform sampler2D uSpecular;
layout (binding = 2) uniform sampler2D uEmission;

out vec4 finalColor;

void main() {
	finalColor = texture(uDiffuse, fs_in.textureCoord);
}