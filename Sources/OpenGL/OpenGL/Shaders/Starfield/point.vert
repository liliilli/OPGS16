#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float uTime;
uniform mat4 projection;

flat out VS_OUT {
	vec4 starColor;
} vs_out;

void main() {
	vec4 newVertex = aPos;
}
