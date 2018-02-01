#include "pp_convex.h"

void shading::PpEffectConvex::Initiate() {
	InsertFrameBuffer(0);
	/** Color Buffer and texture */
	InsertColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	auto& texture_0 = GetTexture(0);
	texture_0->SetTextureParameterI({
		{GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
		{ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER },{ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER } });
	texture_0->SetBorderColor({ 0, 0, 0, 1 });
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	/** The rest */
	InitiateDefaultDepthBuffer();
	InsertUniformValue("uIntensity", 0.05f);
	InitiateShader("Convex", "Shaders/Global/convex.frag");

	/** Last */
	PostProcessingFrame::Initiate();
}
