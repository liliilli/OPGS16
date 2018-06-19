#version 430 core
layout (location = 0) in vec3 iVertex;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

out VS_OUT {
	vec2 texCoord;
} vs_out;

uniform mat4 projection;

void main() {
	gl_Position = projection * vec4(iVertex.xy, 0, 1);
	vs_out.texCoord = iTexCoord;
}