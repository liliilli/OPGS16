#version 430 core
out vec4 color;

in VS_OUT {
	vec2 texCoord;
	vec2 position;
} fs_in;

layout (binding = 0) uniform sampler2D uTexture0;
uniform float uIntensity;
uniform float uInterval;
uniform float uMove;

void main(void) {
	float x_offset = uIntensity * sin( (fs_in.texCoord.y + uMove) / uInterval ) / 100.0f;

	vec2 finalTexel = fs_in.texCoord + vec2(x_offset, 0);
	color = vec4(vec3(texture(uTexture0, finalTexel)), 1);
}