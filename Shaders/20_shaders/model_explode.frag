#version 330 core
out vec4 FragColor;
in vec2 fTexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;

void main() {
	vec3 diffuse = vec3(texture(texture_diffuse1, fTexCoord));

	FragColor = vec4(diffuse, 1.0);
}