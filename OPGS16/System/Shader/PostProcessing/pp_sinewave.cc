#include "pp_sinewave.h"
#include <GLFW\glfw3.h>

namespace shading {

void PpEffectSinewave::Initiate() {
	InsertFrameBuffer(0);
	/** Color buffer and texture */
	InsertColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	auto& texture_0 = GetTexture(0);
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	/** The rest */
	InitiateDefaultDepthBuffer();
	InsertUniformValue("uIntensity", 32.f);
	InsertUniformValue("uInterval", 1.f);
	InsertUniformValue("uMove", 0.f);
	InitiateShader("SineWave", "Shaders/Global/sinewave.frag");

	/** Last */
	PostProcessingFrame::Initiate();
}

void PpEffectSinewave::Update() {
	ReplaceUniformValue("uMove", static_cast<float>(glfwGetTime()));
}

}