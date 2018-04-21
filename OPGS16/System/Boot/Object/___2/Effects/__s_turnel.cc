
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/Object/___2/Public/__s_starfield.cc
 * @author Jongmin Yun
 * @log
 * 2018-04-21 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "__s_turnel.h"    /*! Header file */

namespace opgs16::builtin::sample {

void __S_TURNEL_FRAMEBUFFER::Initialize() {
	GenerateFrameBuffer(0);
	/*! Color buffer and texture */
	GenerateColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);

	/*! The rest */
	InitializeDefaultDepthBuffer();
	SetShader("__s_turnel");

	/** Last */
	CPostProcessingFrame::Initialize();
}

} /*! opgs16::builtin::turnel */