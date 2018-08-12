#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Shader/PostProcessing/pp_gray.h
/// @brief Screen gray screen post-processing effect.
/// @author Jongmin Yun
/// @log
/// 2018-02-03 Create file.
/// 2018-04-20 Move class to ::opgs16::builtin::postprocessing, and remove ::shading namespace.
/// 2018-04-21 Change base class CFrameBufferFrame to CPostProcessingFrame.
/// 2018-08-21 Move implements to .cc file.
///

/// Header file
#include <Shader/PostProcessing/pp_gray.h>

namespace opgs16::builtin::postprocessing {

void PpEffectGray::Initialize() {
  GenerateFrameBuffer(0);
  InitializeDefaultDepthBufferToFrameBuffer(0);
  GenerateDefaultColorBuffer();
  BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);

  InsertUniformValue("uIntensity", 1.0f);
  SetShader("ppGray");

  CFrameBufferFrame::Initialize();
}

} /// ::opgs16::builtin::postprocessing namespace