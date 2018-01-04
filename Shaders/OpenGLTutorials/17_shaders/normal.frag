#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoord;

uniform vec3 cameraPos;
uniform samplerCube skybox;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;

void main() {
	vec3 diffuse = vec3(texture(texture_diffuse1, TexCoord));
	vec3 ref = vec3(texture(texture_reflection1, TexCoord));

    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 sky = vec3(texture(skybox, R));

	vec3 result = diffuse + (ref * sky);
	//vec3(texture(skybox, R));

	FragColor = vec4(result, 1.0);
}