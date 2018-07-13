#version 430 core

layout (location = 0) in vec3 opPosition;

out VS_OUT {
	vec2 texCoord;
} vs_out;

uniform mat4	opProj;

uniform int		uNumber;
uniform int		uPos[64];
uniform float   uYScale;

void main() {
	const float width   = 178.f;
	const float x_width = 1.0f / uNumber;

	mat4 model;
	model[0][0]			= width / 2; model[1][1] = uYScale;
	float world_x		= width / (uNumber << 1) * ((gl_InstanceID << 1) + 1);
	model[3]			= vec4(39 + world_x, 112 + uPos[gl_InstanceID], 0, 1);

	const vec2 local	= vec2(opPosition.x / uNumber, opPosition.y);
	gl_Position			= opProj * model * vec4(local, 0, 1);

	const vec2 offset	= ((opPosition.xy + 1) / 2) * vec2(x_width, 1);
	const vec2 texel	= vec2(x_width * gl_InstanceID, 0) + offset;
	vs_out.texCoord		= texel;
}