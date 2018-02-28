#version 430 core

out VS_OUT {
	vec2 texCoord;
} vs_out;

uniform mat4 projection;
uniform vec2 uTexIndex;	/* (n, m) */
uniform	vec2 uWHSize;	/* Coordinate value [0, 1] float. (w, h) */

vec2 GetTexCoord(const vec2 pos);

void main() {
	const vec2 vertices[] = vec2[](
		vec2(-1, -1), vec2(1, -1), vec2(-1, 1),
		vec2(-1, 1), vec2(1, -1), vec2(1, 1)
	);

	gl_Position		= projection * vec4(vertices[gl_VertexID], 0, 1);
	vs_out.texCoord = GetTexCoord(vertices[gl_VertexID]);
}

vec2 GetTexCoord(const vec2 pos) {
	vec2 col_offset	= (pos + vec2(1, -1)) / 2.0f;
	col_offset.y	= abs(col_offset.y);

	vec2 tex_coord  = uWHSize * (uTexIndex + col_offset);
	tex_coord.y		= 1 - tex_coord.y;

	return tex_coord;
}