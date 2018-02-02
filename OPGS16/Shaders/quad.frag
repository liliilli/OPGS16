#version 430 core
out vec4 color;
in vec2 texCoord;

void main() {
	color = vec4(1, texCoord.t, texCoord.s, 1);
}