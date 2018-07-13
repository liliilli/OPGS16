#version 430 core

out vec4 color;
in VS_OUT {
	vec2 texCoord;
} fs_in;

layout (binding = 0) uniform sampler2D uTexture1;
uniform float alpha;

void main() {
	color = texture(uTexture1, fs_in.texCoord) * vec4(alpha);
}
