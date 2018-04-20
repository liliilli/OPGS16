#ifndef OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H
#define OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Shader/PostProcessing/pp_scaling.h
 * @brief Screen texture scaling post-processing effect.
 * @author Jongmin Yun
 * @log
 * 2018-04-20 Move class to ::opgs16::builtin::postprocessing, and remove ::shading namespace.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../pp_frame.h"    /*! ::opgs16::element::CPostProcessingFrame */

namespace opgs16::builtin::postprocessing {

/**
 * @class PpEffectScaling
 * @brief Screen texture scaling post-processing effect.
 * This effect must be actived by default, or application can not use scaling option.
 */
class PpEffectScaling final : public element::CPostProcessingFrame {
public:
	/**
	 * @brief This functions must be called in initiating PostProcessingFrame instance.
	 * This makes it operable to use in rendering time. Otherwise, this does not render anything.
	 * If you didn't call this method in creating, Undefined rendering behavior will occur.
	 *
	 * This function can be overriden by Derived class.
	 * Derived class will just use this method to initialize all settings of PostProcessingFrame.
	 */
	void Initialize() override final;

	/**
	 * @brief Render texture and components.
	 * This must be called after arbitary frame buffer bound.
	 * This methods could be overriden by derived class.
	 */
    void RenderEffect() override final;
};

} /*! opgs16::builtin::postprocessing */

#endif /** OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H */