#version 430 core

in VS_OUT {
	vec2 texCoord;
} fs_in;

out vec4 finalColor;

layout (binding = 0) uniform sampler2D uTexture;

void main() {
	finalColor = vec4(vec3(texture(uTexture, fs_in.texCoord)), 1);
}
