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

#include "../../Element/Public/postprocessing_frame.h"  /*! ::opgs16:element::CPostProcessingFrame */

namespace opgs16::builtin::postprocessing {

class PpEffectGray final : public element::CPostProcessingFrame {
	void Initialize() override final {
		GenerateFrameBuffer(0);

		GenerateColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
		BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
		InitializeDefaultDepthBuffer();
		InsertUniformValue("uIntensity", 1.0f);
		SetShader("ppGray");

		CFrameBuferFrame::Initialize();
	}
};

} /*! ::opgs16::builtin::postprocessing */

#endif /** OPGS16_SYSTEM_SHADER_POST_PROCESSING_PP_SCALING_H */
