#version 430 core

out VS_OUT {
	vec2 texCoord;
} vs_out;

uniform mat4 projection;

void main() {
	const vec2 vertices[] = vec2[](
		vec2(-1, -1), vec2(1, -1), vec2(-1, 1),
		vec2(-1, 1), vec2(1, -1), vec2(1, 1)
	);

	gl_Position		= projection * vec4(vertices[gl_VertexID], 0, 1);
	vs_out.texCoord = (vertices[gl_VertexID] + vec2(1)) / 2;
}