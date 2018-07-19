#version 430 core

uniform vec3 uColor;

out vec4 color;

void main() {
	color = vec4(uColor, 1);
}