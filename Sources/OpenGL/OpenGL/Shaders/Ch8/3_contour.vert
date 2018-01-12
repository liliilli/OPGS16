#version 430 core

void main() {
	vec3 vertices[] = vec3[](
		vec3(-1, -1, 0),
		vec3( 1, -1, 0),
		vec3(-1,  1, 0),

		vec3(-1,  1, 0),
		vec3( 1, -1, 0),
		vec3( 1,  1, 0)
	);

	gl_Position = vec4(vertices[gl_VertexID] * 0.5, 1);
}