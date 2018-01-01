#version 330 core
out vec3 Normal;
out vec3 FragmentPos;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 modelNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

	FragmentPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(modelNormal)) * aNormal;
}
