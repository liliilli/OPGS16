#version 430 core

layout (location = 0) in vec3 opPosition;
layout (location = 1) in vec3 opNormal;
layout (location = 2) in vec3 opTangent;
layout (location = 3) in vec2 opTexCoord;

uniform mat4 opProj;
uniform mat4 opView;
uniform mat4 opModel;

out VS_OUT {
	vec2 texCoord;
} vs_out;

void main() {
	gl_Position	= opProj * opView * opModel * vec4(opPosition.xy, 0, 1);
	vs_out.texCoord = opTexCoord;
};