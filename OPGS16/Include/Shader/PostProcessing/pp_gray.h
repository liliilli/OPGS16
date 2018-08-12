#ifndef OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H
#define OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Shader/PostProcessing/pp_gray.h
 * @brief Screen gray screen post-processing effect.
 * @author Jongmin Yun
 * @log
 * 2018-02-03 Create file.
 * 2018-04-20 Move class to ::opgs16::builtin::postprocessing, and remove ::shading namespace.
 * 2018-04-21 Change base class CFrameBufferFrame to CPostProcessingFrame.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/// ::opgs16:element::CPostProcessingFrame
#include <Element/postprocessing_frame.h>

namespace opgs16::builtin::postprocessing {

class PpEffectGray final : public element::CPostProcessingFrame {
	void Initialize() override final;
};

} /*! ::opgs16::builtin::postprocessing */

#endif /** OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H */
