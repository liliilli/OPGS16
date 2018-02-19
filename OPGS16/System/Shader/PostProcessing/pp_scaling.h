#ifndef OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H
#define OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H

/**
 * @file System/Shader/pp_scaling.h
 * @brief Screen texture scaling post-processing effect.
 * Last Updated :: 2018 - 02 - 02
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include "..\pp_frame.h"

/**
 * @namespace shading
 * @brief shading namespace is for m_object_list related to shading and shader management.
 */
namespace shading {

/**
 * @class PpEffectScaling
 * @brief Screen texture scaling post-processing effect.
 * This effect must be actived by default, or application can not use scaling option.
 */
class PpEffectScaling final : public PostProcessingFrame {
public:
	/**
	 * @brief This functions must be called in initiating PostProcessingFrame instance.
	 * This makes it operable to use in rendering time. Otherwise, this does not render anything.
	 * If you didn't call this method in creating, Undefined rendering behavior will occur.
	 *
	 * This function can be overriden by Derived class.
	 * Derived class will just use this method to initialize all settings of PostProcessingFrame.
	 */
	virtual void Initiate() override final;

	/**
	 * @brief Render texture and components.
	 * This must be called after arbitary frame buffer bound.
	 * This methods could be overriden by derived class.
	 */
    virtual void RenderEffect() override final;
};

}

#endif /** OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H */