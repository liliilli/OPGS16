#version 330 core
out vec4 FragColor;
in	vec2 TexCoord;
in	vec3 FragmentPos;
in	vec3 Normal;

// Uniform variables
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {    
	FragColor = vec4(1.000, 0.514, 0.400, 1.0);
}
