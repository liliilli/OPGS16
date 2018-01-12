#version 430 core

layout (location = 0) in vec4 position;
uniform mat4 mv;

out VS_OUT {
	vec2 texCoord;
} vs_out;

void main() {
	vs_out.texCoord = (vec2(position) + 1.0) * 0.5f;

	gl_Position = mv * position;
}