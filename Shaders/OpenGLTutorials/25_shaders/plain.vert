#version 330 core
out vec2 fTexCoord;

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

void main() {
    gl_Position = vec4(aPos.xyz, 1.0f);
	fTexCoord = aTexCoord;
}
