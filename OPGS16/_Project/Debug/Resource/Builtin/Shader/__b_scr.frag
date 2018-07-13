#version 430 core

out vec4 color;
in VS_OUT { vec2 texCoord; } fs_in;

uniform vec2 uTexelLD;
uniform vec2 uTexelRU;

layout (binding = 0) uniform sampler2D uTexture1;
uniform float opAlpha;

void main() {
	const vec4 alpha_vec = vec4(1, 1, 1, opAlpha);
	color = texture(uTexture1, uTexelLD + (fs_in.texCoord * (uTexelRU - uTexelLD))) * alpha_vec;
}