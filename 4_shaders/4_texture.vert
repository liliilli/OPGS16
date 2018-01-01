#version 330 core
out vec3 ourColor;
out vec2 TexCoord;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    ourColor = aColor;
    TexCoord = aTexCoord;
}