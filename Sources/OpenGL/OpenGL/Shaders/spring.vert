#version 430 core

layout (location = 0) in vec4 position_mass;
layout (location = 1) in vec3 velocity;
layout (location = 2) in ivec4 connection;

layout (binding = 0) uniform samplerBuffer texPosition;

out vec4 tfPositionMass;
out vec3 tfVelocity;

uniform float t = 0.07;	// Time interval
uniform float k = 7.1;	// Spring factor

const vec3 gravity = vec3(0, -0.08, 0);

uniform float c = 2.8;	// Global damping factor
uniform float rest_length = 0.88;

void main(void) {
	vec3 p = position_mass.xyz;
	float m = position_mass.w;
	vec3 u = velocity;

	vec3 F = gravity * m - c * u;
	bool fixed_node = true;

	for (int i = 0; i < 4; ++i) {
		if (connection[i] != -1) {
			vec3 q = texelFetch(texPosition, connection[i]).xyz;
			vec3 d = q - p;
			float x = length(d);

			F += -k * (rest_length - x) * normalize(d);
			fixed_node = false;
		}
	}

	if (fixed_node == true)
		F = vec3(0);

	vec3 a = F / m;
	vec3 s = u * t + 0.5f * a * pow(t, 2);
	vec3 v = u + a * t;

	s = clamp(s, vec3(-25.0), vec3(25.0));

	// Save results
	tfPositionMass = vec4(p + s, m);
	tfVelocity = v;

	//gl_Position = vec4(p + s, 1);
}