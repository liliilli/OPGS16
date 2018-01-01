#version 330 core
out vec2 TexCoord;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 rotation;

void main() {
    gl_Position = rotation * vec4(aPos.xyz, 1.0f);
	TexCoord = aTexCoord;
}
