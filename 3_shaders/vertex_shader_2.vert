#version 330 core
out vec3 outColor;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Uniform Variable
uniform float x_offset;

void main() {
   gl_Position = vec4(x_offset + aPos.x, aPos.y, aPos.z, 1.0);
   outColor = aColor;
}