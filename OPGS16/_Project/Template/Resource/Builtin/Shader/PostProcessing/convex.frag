#version 430 core

out vec4 color;

in VS_OUT {
	vec2 texCoord;
	vec2 position;
} fs_in;

layout (binding = 0) uniform sampler2D uTexture0;
uniform float uIntensity;
uniform float uThreshold = 0.85f;
uniform float uMax = 64.0f;

float get_over_threshold_intensity(const float a, const float t) {
	float b = pow(t, 2) * (1 - (1 / uMax));
	return uMax * pow(a - (t - (t / uMax)), 2) + b;
}

void main(void) {
	float x = fs_in.position.x;
	float y = fs_in.position.y;

	float x_intensity = uIntensity;
	float y_intensity = uIntensity;
	if (abs(x) >= uThreshold && abs(y) >= uThreshold) {
		y_intensity *= get_over_threshold_intensity(abs(x), uThreshold);
		x_intensity *= get_over_threshold_intensity(abs(y), uThreshold);
	}
	else {
		y_intensity *= pow(x, 2);
		x_intensity *= pow(y, 2);
	}
	// Then,
	float y_offset = y_intensity * y;
	float x_offset = x_intensity * x;

	vec2 finalTexel = fs_in.texCoord + vec2(x_offset, y_offset);
	color = vec4(vec3(texture(uTexture0, finalTexel)), 1);
}