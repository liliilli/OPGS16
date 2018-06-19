#version 430 core
out vec4 color;

in VS_OUT {
	vec2 texCoord;
	vec2 position;
} fs_in;

layout (binding = 0) uniform sampler2D uTexture0;

void main() {
	color = vec4(vec3(texture(uTexture0, fs_in.texCoord)), 1);
}