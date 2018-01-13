#version 430 core

in VS_OUT {
	vec2 texCoord;
} fs_in;

uniform float uExposure;

layout (binding = 0) uniform sampler2D uScene;
layout (binding = 1) uniform sampler2D uBlur;

out vec4 finalColor;

void main() {
	const float gamma = 2.2;
	
	vec3 hdrColor	= vec3(texture(uScene, fs_in.texCoord));
	vec3 bloomColor = vec3(texture(uBlur, fs_in.texCoord));
	hdrColor		+= bloomColor;

	/** Tone mapping */
	vec3 result = vec3(1.0f) - exp(-hdrColor * uExposure);
	/** Gamma correction */
	result = pow(result, vec3(1.0 / gamma));

	finalColor = vec4(result, 1.0f);
}