#include <precompiled.h>
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Shader/PostProcessing/pp_convex.cc
 * @brief Convex monitor post-processing effects.
 * @author Jongmin Yun
 * @log
 * 2018-04-20 Move class to ::opgs16::builtin::postprocessing, and remove ::shading namespace.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Shader\PostProcessing\pp_convex.h>    /// Header file

namespace opgs16::builtin::postprocessing {

void PpEffectConvex::Initialize() {
	GenerateFrameBuffer(0);
	/** Color Buffer and texture */
  GenerateDefaultColorBuffer();
	auto& texture_0 = GetTexture(0);
	texture_0->SetTextureParameterI({
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR }, { GL_TEXTURE_MAG_FILTER, GL_LINEAR },
		{ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER },{ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER } });
	texture_0->SetBorderColor({ 0, 0, 0, 1 });
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	/** The rest */
	InitializeDefaultDepthBufferToFrameBuffer(0);
	InsertUniformValue("uIntensity", 0.05f);
	SetShader("ppConvex");

	/** Last */
	CFrameBufferFrame::Initialize();
}

} /*! ::opgs16::builtin::postprocessing */
