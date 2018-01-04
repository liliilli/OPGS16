#version 330 core
out vec4 FragColor;

// We set this variable in the OpenGL mode.
uniform vec4 outColor; 

void main() {
   FragColor = outColor;
}