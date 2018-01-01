#version 330 core
out vec4 FragColor;
in	vec2 TexCoord;

uniform sampler2D screenTexture;

const float offset = 1.0f / 300.0f;
void main() {
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	vec2(0, offset),	vec2(offset, offset),
		vec2(-offset, 0),		vec2(0, 0),			vec2(offset, 0),
		vec2(-offset, -offset),	vec2(0, -offset),	vec2(offset, -offset)
	);
	
	float kernel[9] = float[](
		 1,  2,  1,
		 2,  4,  2,
		 1,  2,  1
	);

	vec3 result = vec3(0);
	for (int i = 0; i < 9; ++i) {
		result += vec3(kernel[i] * texture(screenTexture, TexCoord + offsets[i])) / 16.0f;
	}

	FragColor = vec4(result, 1.0);
}
