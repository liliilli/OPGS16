#version 430 core

void main() {
	const vec4 vertices[] = vec4[](
		vec4( 0.25, -0.25, 0.5, 1.0),
		vec4(-0.25, -0.25, 0.5, 1.0),
		vec4( 0.25,  0.25, 0.5, 1.0));

	gl_Position = vertices[gl_VertexID] * vec4(2, 2, 1, 1);
}