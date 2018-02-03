#version 430 core
out vec4 color;

in VS_OUT {
	vec2 texCoord;
	vec2 position;
} fs_in;

layout (binding = 0) uniform sampler2D uTexture0;
uniform float uIntensity; // [0, 1]

float linear(float color, float gray) {
	return color * (1 - uIntensity) + gray * uIntensity;
}

void main() {
	vec3 colorValue = vec3(texture(uTexture0, fs_in.texCoord));
	float gray		= 0.299 * colorValue.r + 0.587 * colorValue.g + 0.114 * colorValue.b;

	vec3 finalColor = vec3(linear(colorValue.r, gray),
						   linear(colorValue.g, gray),
						   linear(colorValue.b, gray));
	color = vec4(finalColor, 1);
}