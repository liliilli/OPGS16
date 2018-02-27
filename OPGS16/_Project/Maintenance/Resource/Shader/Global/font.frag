#version 430 core

out vec4 color;
in VS_OUT {
	vec2 texCoord;
} fs_in;

layout (binding = 0) uniform sampler2D text;
uniform vec3 textColor;

void main() {
	color = vec4(textColor, 1) * vec4(1, 1, 1, texture(text, fs_in.texCoord).r);
}
