#ifndef OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SINEWAVE_H
#define OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SINEWAVE_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Shader/PostProcessing/pp_convex.h
 * @brief Convex monitor post-processing effects.
 * @author Jongmin Yun
 * @log
 * 2018-04-20 Move class to ::opgs16::builtin::postprocessing, and remove ::shading namespace.
 * 2018-04-21 Change base class CFrameBufferFrame to CPostProcessingFrame.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/// ::opgs16:element::CPostProcessingFrame
#include <Element\postprocessing_frame.h>

namespace opgs16::builtin::postprocessing {

/**
 * @class PpEffectConvex
 * @brief Convex post-processing effect.
 */
class PpEffectSinewave final : public element::CPostProcessingFrame {
	/**
	 * @brief This functions must be called in initiating PostProcessingFrame instance.
	 * This makes it operable to use in rendering time. Otherwise, this does not render anything.
	 * If you didn't call this method in creating, Undefined rendering behavior will occur.
	 *
	 * This function can be overriden by Derived class.
	 * Derived class will just use this method to initialize all settings of PostProcessingFrame.
	 */
	void Initialize() override final;

	void Update() override final;
};

} /*! opgs16::builtin::postprocessing */

#endif /** OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SINEWAVE_H */