#version 330 core
out vec4 FragColor;
in	vec3 TexCoord;

uniform samplerCube cubemap;

void main() {    
	FragColor = texture(cubemap, TexCoord);
}
