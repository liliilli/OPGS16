#version 330 core
out vec3 Normal;
out vec3 FragmentPos;
out vec3 LightPos;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform vec3 lightPos;		// World space coord
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

	Normal = mat3(transpose(inverse(view * model))) * aNormal;
	FragmentPos = vec3(view * model * vec4(aPos, 1.0));
	LightPos = vec3(view * vec4(lightPos, 1.0));
}
