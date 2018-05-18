#version 430 core
layout (location = 0) in vec4 info; // vec2 pos, vec2 tex

out VS_OUT {
	vec2 texCoord;
} vs_out;

uniform mat4 projection;

void main() {
	gl_Position = projection * vec4(info.xy, 0, 1);
	vs_out.texCoord = info.zw;
}
