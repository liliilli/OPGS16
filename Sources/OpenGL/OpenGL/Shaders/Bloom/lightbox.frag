#version 430 core

in VS_OUT {
	vec3 fragmentPos;
	vec3 normalDir;
	vec2 textureCoord;
} fs_in;

uniform vec3 uColor;

out vec4 finalColor;

void main() {
	finalColor = vec4(uColor, 1.0f);
}