#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec2 fTexCoord;
in VS_OUT {
	vec3 Normal;
	vec2 TexCoord;
} gs_in[];

uniform float time;


vec3 GetNormal() {
	return gs_in[0].Normal;
}

vec4 explode(vec4 position, vec3 normal) {
	float magnitude = 2.0f;
	vec3 direction = normal * ((sin(time * 2) + 1.0) / 2.0) * magnitude;

	return position + vec4(direction, 0.0);
}

void main() {
	vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    fTexCoord = gs_in[0].TexCoord;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    fTexCoord = gs_in[1].TexCoord;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    fTexCoord = gs_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}