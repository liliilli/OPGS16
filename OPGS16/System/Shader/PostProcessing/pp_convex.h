#ifndef OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_CONVEX_H
#define OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_CONVEX_H

/**
 * @file System/Shader/pp_convex.h
 * @brief Convex monitor post-processing effects.
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include "..\pp_frame.h"

/**
 * @namespace shading
 * @brief shading namespace is for objects related to shading and shader management.
 */
namespace shading {

/**
 * @class PpEffectConvex
 * @brief Convex post-processing effect.
 */
class PpEffectConvex : public PostProcessingFrame {
	/**
	 * @brief This functions must be called in initiating PostProcessingFrame instance.
	 * This makes it operable to use in rendering time. Otherwise, this does not render anything.
	 * If you didn't call this method in creating, Undefined rendering behavior will occur.
	 *
	 * This function can be overriden by Derived class.
	 * Derived class will just use this method to initialize all settings of PostProcessingFrame.
	 */
	[[noreturn]] virtual void Initiate() override final;

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

#endif /** OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_CONVEX_H */