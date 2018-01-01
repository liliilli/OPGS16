#version 330 core
out vec4 FragColor;
in	vec2 TexCoord;
in	vec3 FragmentPos;
in	vec3 Normal;

// Uniform variables
uniform sampler2D texture_diffuse1;

void main() {
	vec4 texColor = texture(texture_diffuse1, TexCoord);
	FragColor = texColor;
}
