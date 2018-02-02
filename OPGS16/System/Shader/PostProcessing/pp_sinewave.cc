#include "pp_sinewave.h"

namespace shading {

void PpEffectSinewave::Initiate() {
	InsertFrameBuffer(0);
	/** Color buffer and texture */
	InsertColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	auto& texture_0 = GetTexture(0);
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	/** The rest */
	InitiateDefaultDepthBuffer();
	InsertUniformValue("uIntensity", 50.f);
	InsertUniformValue("uInterval", 1.f);
	//InsertUniformValue("uMove", static_cast<float>(glfwGetTime()));
	InsertUniformValue("uMove", 0.f);
	InitiateShader("SineWave", "Shaders/Global/sinewave.frag");

	/** Last */
	PostProcessingFrame::Initiate();
}

}