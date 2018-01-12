#version 430 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out VS_OUT {
	vec3 fragmentPos;
	vec3 normalDir;
	vec2 textureCoord;
} vs_out;

void main() {
	vs_out.fragmentPos	= vec3(uModel * vec4(iPos, 1.0f));
	vs_out.normalDir	= normalize(mat3(uModel) * iNormal);
	vs_out.textureCoord = iTexCoord;

	gl_Position = uProjection * uView * vec4(vs_out.fragmentPos, 1.0f);
}