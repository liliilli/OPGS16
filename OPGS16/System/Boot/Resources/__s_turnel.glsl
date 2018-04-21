#version 430 core

#define sqrt2 1.4142135f

in VS_OUT {
	vec2 texCoord;
	vec2 position;
} fs_in;

layout (binding = 0) uniform sampler2D uTexture0;

void main(void) {
	const float PI = 3.1415926535897932384626433832795;
	const float PI2 = 6.283185307179586476925286766559;
	const vec2 std_pos = vec2(0, -1.4142135f);

	const float pos_length = length(fs_in.position);
	const float value  = clamp(dot(std_pos, fs_in.position) / (pos_length * sqrt2), -1.f, 1.f);
	float angle_radian = acos(value);

	const float offset = floor(fs_in.position.x);
	angle_radian   = offset * (-PI2) + angle_radian - (2 * offset * angle_radian);

	const float u  = angle_radian / PI2;
	const float v  = (sqrt2 - pos_length) / sqrt2;
	const float alpha = pow(clamp(pos_length, 0.f, 1.f), 1.5f) * 0.5;

	gl_FragColor   = vec4(texture(uTexture0, vec2(u, v)) * vec4(alpha, alpha, alpha, 1));
}
