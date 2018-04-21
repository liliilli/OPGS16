
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

#include "__s_starfield.h"    /*! Header file */
#include <GLFW/glfw3.h>

namespace opgs16::builtin::sample {

void __S_STARFIELD_FRAMEBUFFER::Initialize() {
	GenerateFrameBuffer(0);
	/*! Color buffer and texture */
	GenerateColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	/*! The rest */
    InsertUniformValue("uTime", 0.f);
    InsertUniformValue("uSpeed", 5.f);
	InitializeDefaultDepthBuffer();

	SetShader("__s_starfield");

	/** Last */
	CPostProcessingFrame::Initialize();
}

void __S_STARFIELD_FRAMEBUFFER::Update() {
	ReplaceUniformValue("uTime", static_cast<float>(glfwGetTime()));
}

} /*! opgs16::builtin::postprocessing */