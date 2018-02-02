#version 430 core

out VS_OUT {
	vec2 tessCoord;
} vs_out;

void main(void) {
	const vec3 vertices[] = vec3[](
		vec3(-.5, 0, -.5), vec3( .5, 0, -.5),
		vec3(-.5, 0,  .5), vec3( .5, 0,  .5)
	);

	int x = gl_InstanceID & 63;
	int y = gl_InstanceID >> 6;
	vec2 offset = vec2(x, y);

	vs_out.tessCoord = (vertices[gl_VertexID].xz + offset + vec2(.5)) / 64.0;
	gl_Position = vec4(vertices[gl_VertexID], 1) + vec4(float(x - 32), 0, float(y - 32), 0);
}
