#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat3 fTBN;
out vec3 fFragPos;
out vec2 fTexCoord;
out vec3 fNormal;
out mat4 fModel;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	
	fTexCoord = aTexCoord;
	fFragPos = vec3(model * vec4(aPos, 1.0));
	fNormal = vec3(model * vec4(aNormal, 0));

	vec3 T = normalize(vec3(model * vec4(aTangent, 0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0)));
	fTBN = mat3(T, B, N);

	fModel = inverse(model);
}
