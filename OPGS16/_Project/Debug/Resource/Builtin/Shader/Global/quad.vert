#version 430 core

out VS_OUT {
	vec2 texCoord;
	vec2 position;
} vs_out;

void main(void) {
	const vec2 vertices[] = vec2[](
		vec2(-1, -1), vec2(1, -1), vec2(-1, 1),
		vec2(-1, 1), vec2(1, -1), vec2(1, 1)
	);

	gl_Position = vec4(vertices[gl_VertexID], 0, 1);
	vs_out.position = vertices[gl_VertexID];
	vs_out.texCoord = (vertices[gl_VertexID] + 1) / 2.0f;
}