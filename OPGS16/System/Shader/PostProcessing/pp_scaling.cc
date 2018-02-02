#include "pp_scaling.h"
#include "..\..\..\application.h"

namespace shading {

void PpEffectScaling::Initiate() {
	InsertFrameBuffer(0);

	auto size = Application::getInstance().GetDefaultScreenSize();
	InsertColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT, size[0], size[1]);
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	InitiateShader("SineWave", "Shaders/Global/quad.frag");

	PostProcessingFrame::Initiate();
}

}