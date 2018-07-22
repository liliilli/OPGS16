#version 430 core

layout (binding = 0) uniform sampler2D opTexture0;

uniform vec2 opTexelLD = vec2(0, 0);
uniform vec2 opTexelRU = vec2(1, 1);
uniform vec2 opScale   = vec2(1, 1);
uniform vec2 opOffset  = vec2(0, 0);
uniform float opAlpha  = 1.0f;

out vec4 color;

in VS_OUT {
	vec2 texCoord;
} fs_in;

vec2 GetTransformedTexel() {
	vec2 uTexelRegion = opTexelRU - opTexelLD;
	vec2 finalTexel = mod((fs_in.texCoord / opScale), 1.0);
	finalTexel += opOffset;

	return opTexelLD + mod(finalTexel, 1.0) * uTexelRegion;
};

void main() {
	color = texture(opTexture0, GetTransformedTexel()) *
			vec4(1, 1, 1, opAlpha);
}