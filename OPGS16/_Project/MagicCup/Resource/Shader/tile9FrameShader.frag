#version 430 core

layout (binding = 0) uniform sampler2D opTexture0;

uniform vec2 opTexelLD = vec2(0, 0);
uniform vec2 opTexelRU = vec2(1, 1);
uniform float opAlpha  = 1.0f;

uniform int  uEdgeSize;
uniform vec2 uPresentSize;

out vec4 color;

in VS_OUT {
	vec2 texCoord;
} fs_in;

vec2 GetTransformedTexel() {
	vec2 fin_texcoord = fs_in.texCoord;

	vec2 tx_ld_thre = vec2(uEdgeSize) / uPresentSize;
	if (fs_in.texCoord.x <= tx_ld_thre.x) {
		fin_texcoord.x = fs_in.texCoord.x / tx_ld_thre.x * 0.25f;
	}
	else if (fs_in.texCoord.x >= (1 - tx_ld_thre.x)) {
		fin_texcoord.x = 1 - ((1 - fs_in.texCoord.x) / tx_ld_thre.x * 0.25f);
	}
	else {
		fin_texcoord.x = fs_in.texCoord.x / tx_ld_thre.x * 0.25f;
		fin_texcoord.x = mod(fin_texcoord.x - 0.25f, 0.50f) + 0.25f;
	}

	if (fs_in.texCoord.y <= tx_ld_thre.y) {
		fin_texcoord.y = fs_in.texCoord.y / tx_ld_thre.y * 0.25f;
	}
	else if (fs_in.texCoord.y >= (1 - tx_ld_thre.y)) {
		fin_texcoord.y = 1 - ((1 - fs_in.texCoord.y) / tx_ld_thre.y * 0.25f);
	}
	else {
		fin_texcoord.y = fs_in.texCoord.y / tx_ld_thre.y * 0.25f;
		fin_texcoord.y = mod(fin_texcoord.y - 0.25f, 0.50f) + 0.25f;
	}

	vec2 uTexelRegion = opTexelRU - opTexelLD;
	return opTexelLD + fin_texcoord * uTexelRegion;
};

void main() {
	color = texture(opTexture0, GetTransformedTexel()) *
			vec4(1, 1, 1, opAlpha);
}