
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Shader/PostProcessing/pp_scaling.cc
 * @author Jongmin Yun
 * @log
 * 2018-04-20 Move class to ::opgs16::builtin::postprocessing, and remove ::shading namespace.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Shader\PostProcessing\pp_scaling.h>   /// Header file
#include <Core\application.h>
#include <Core\core_setting.h>

namespace opgs16::builtin::postprocessing {

void PpEffectScaling::Initialize() {
	GenerateFrameBuffer(0);
  GenerateDefaultColorBuffer();
	BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	InitializeDefaultDepthBuffer();
	SetShader("ppQuad");

	CFrameBuferFrame::Initialize();
}

void PpEffectScaling::RenderEffect() {
  using opgs16::setting::GetScaleValueIntegerOf;
  using opgs16::setting::GetScreenWidth;
  using opgs16::setting::GetScreenHeight;

  const int32_t scale_val = GetScaleValueIntegerOf();
  glViewport(
      0, 0,
      GetScreenWidth()  * scale_val,
      GetScreenHeight() * scale_val
  );

  CFrameBuferFrame::RenderEffect();
}

} /*! opgs16::builtin::postprocessing */