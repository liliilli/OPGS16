#version 430 core

layout (binding = 0) uniform sampler2D opTexture0;

uniform vec2 opTexelLD = vec2(0, 0);
uniform vec2 opTexelRU = vec2(1, 1);
uniform float opAlpha = 1.0f;

out vec4 color;

in VS_OUT { vec2 texCoord; } fs_in;

void main() {
	const vec4 alpha_vec = vec4(1, 1, 1, opAlpha);
	color =
		texture(opTexture0, opTexelLD + (fs_in.texCoord * (opTexelRU - opTexelLD))) * alpha_vec;
}