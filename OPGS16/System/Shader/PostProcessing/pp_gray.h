#ifndef OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H
#define OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H

// @file System/Shader/pp_scaling.h
// @brief Screen gray screen post-processing effect.
// Last Updated :: 2018 - 02 - 03
//
// @author Jongmin Yun
// @version 0.0.1
#include "..\pp_frame.h"

// @namespace shading
// @brief shading namespace is for objects related to shading and shader management.
namespace shading {

// @class PpEffectConvex
// @brief Convex post-processing effect.
class PpEffectGray : public PostProcessingFrame {
	/**
	 * @brief This functions must be called in initiating PostProcessingFrame instance.
	 * This makes it operable to use in rendering time. Otherwise, this does not render anything.
	 * If you didn't call this method in creating, Undefined rendering behavior will occur.
	 *
	 * This function can be overriden by Derived class.
	 * Derived class will just use this method to initialize all settings of PostProcessingFrame.
	 */
	[[noreturn]] virtual void Initiate() override final {
		InsertFrameBuffer(0);

		InsertColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
		BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
		InitiateDefaultDepthBuffer();
		InsertUniformValue("uIntensity", 1.0f);
		InitiateShader("Gray");

		PostProcessingFrame::Initiate();
	}

	/**
	* @brief Render texture and components.
	* This must be called after arbitary frame buffer bound.
	* This methods could be overriden by derived class.
	*/
	[[noreturn]] virtual void RenderEffect() override final {
		PostProcessingFrame::RenderEffect();
	}
};

}

#endif /** OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H */
