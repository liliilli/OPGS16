#version 430 core

out vec4 color;
layout (binding = 0) uniform sampler2D texture_check;

in TES_OUT {
	vec3 N;
	vec2 texCoord;
} fs_in;

const vec4 R = vec4(-1, 0, 1, 0);
const vec4 B = vec4( 1, 0, -1, 0);

void main() {
	vec3 N = normalize(fs_in.N);
	vec4 c = texture(texture_check, fs_in.texCoord) * -N.z + vec4(0, 0, 0, 1);

	color = clamp(c , vec4(0), vec4(1));
}