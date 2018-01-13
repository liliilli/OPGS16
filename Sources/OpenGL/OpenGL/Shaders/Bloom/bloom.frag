#version 430 core

in VS_OUT {
	vec2 texCoord;
} fs_in;

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.194595, 0.121622, 0.054054, 0.016216);

layout (binding = 0) uniform sampler2D uTexture;

layout (location = 0) out vec4 finalColor;

void main() {
	/** Get size of single texel */
	vec2 textureOffset = 1.0f / textureSize(uTexture, 0);
	
	/** Current fragment's contribution */
	vec3 result = texture(uTexture, fs_in.texCoord).rgb * weight[0];

	if (horizontal) {
		for (int i = 1; i < 5; ++i) {
			float x = textureOffset.x;
			result += texture(uTexture, fs_in.texCoord + vec2(x * i, 0)).rgb * weight[i];
			result += texture(uTexture, fs_in.texCoord - vec2(x * i, 0)).rgb * weight[i];
		}
	}
	else {
		for (int i = 1; i < 5; ++i) {
			float y = textureOffset.y;
			result += texture(uTexture, fs_in.texCoord + vec2(0, y * i)).rgb * weight[i];
			result += texture(uTexture, fs_in.texCoord - vec2(0, y * i)).rgb * weight[i];
		}
	}

	finalColor = vec4(result, 1.0f);
}