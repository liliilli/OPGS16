#version 430 core

out vec4 color;
uniform vec2 uTexelLD;
uniform vec2 uTexelRU;

in VS_OUT {
	vec2 texCoord;
} fs_in;

layout (binding = 0) uniform sampler2D uTexture1;
uniform float alpha;

void main() {
	color = vec4(texture(uTexture1, uTexelLD + (fs_in.texCoord * (uTexelRU - uTexelLD))).rgb, alpha);
}
