#version 330 core
out vec3 ourColor;
out vec2 TexCoord;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

// angle [ cos(theta) , sin(theta) ]
uniform vec2 angle;

void main() {
    float x = angle.x * aPos.x + angle.y * aPos.y;
    float y = -angle.y * aPos.x + angle.x * aPos.y;

    gl_Position = vec4(x, y, aPos.z, 1.0);

    ourColor = aColor;
    TexCoord = aTexCoord;
}