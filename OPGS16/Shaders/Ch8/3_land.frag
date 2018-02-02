#version 430 core

out vec4 color;

layout (binding = 1) uniform sampler2D tex_color;

in TES_OUT {
	vec2 texCoord;
	float height;
} fs_in;

void main() {
	if (fs_in.height > 5.0f) {
		color = vec4(mix(vec3(1, 0, 0), vec3(0, 1, 0), 2 - fs_in.height / 5.0f), 1);
	}
	else {
		color = vec4(mix(vec3(0, 1, 0), vec3(0, 0, 1), 1 - fs_in.height / 5.0f), 1);
	}
}