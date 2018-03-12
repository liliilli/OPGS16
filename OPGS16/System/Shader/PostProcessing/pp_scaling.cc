#include "pp_scaling.h"             /*! Header file */
#include "../../Core/Public/application.h" /*! opgs16::Application */
#include "../../Core/Public/core_setting.h"

namespace shading {

void PpEffectScaling::Initiate() {
	InsertFrameBuffer(0);

	InsertColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	InitiateDefaultDepthBuffer();
	InitiateShader("Quad");

	PostProcessingFrame::Initiate();
}

void PpEffectScaling::RenderEffect() {
    auto scale_val = opgs16::MApplication::Instance().Setting().ScaleValueIntegerOf();

    glViewport(0, 0, 256 * scale_val, 224 * scale_val);
    PostProcessingFrame::RenderEffect();
}

}