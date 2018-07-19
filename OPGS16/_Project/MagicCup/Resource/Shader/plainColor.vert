#version 430 core

layout (location = 0) in vec3 opPosition;
layout (location = 1) in vec3 opNormal;

uniform mat4 opProj;
uniform mat4 opView;
uniform mat4 opModel;

void main() {
	gl_Position	= opProj * opView * opModel * vec4(opPosition.xy, 0, 1);
};