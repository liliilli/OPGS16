
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Shader/PostProcessing/pp_sinewave.cc
 * @author Jongmin Yun
 * @log
 * 2018-04-20 Move class to ::opgs16::builtin::postprocessing, and remove ::shading namespace.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "pp_sinewave.h"    /*! Header file */
#include <GLFW/glfw3.h>

namespace opgs16::builtin::postprocessing {

void PpEffectSinewave::Initialize() {
	GenerateFrameBuffer(0);
	/** Color buffer and texture */
	GenerateColorBuffer(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	/** The rest */
	InitializeDefaultDepthBuffer();
	InsertUniformValue("uIntensity", 2.f);
	InsertUniformValue("uInterval", 0.05f);
	InsertUniformValue("uMove", 0.f);
	SetShader("ppSineWave");

	/** Last */
	CFrameBuferFrame::Initialize();
}

void PpEffectSinewave::Update() {
	ReplaceUniformValue("uMove", static_cast<float>(glfwGetTime()));
}

} /*! opgs16::builtin::postprocessing */