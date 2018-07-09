#ifndef DEBUG_PROJECT_SCRIPT_RENDERINGTEST_PROCEDURAL2D_H
#define DEBUG_PROJECT_SCRIPT_RENDERINGTEST_PROCEDURAL2D_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace debug::script {

class Procedural2DRenderingTest final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(Procedural2DRenderingTest);

private:
  opgs16::element::CObject* m_subject = nullptr;
  opgs16::element::CObject* m_description = nullptr;
  opgs16::element::CObject* m_selection = nullptr;

  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;
  void Destroy() override final;
};

} /// ::debug::script

#endif /// DEBUG_PROJECT_SCRIPT_RENDERINGTEST_PROCEDURAL2D_H