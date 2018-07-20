#ifndef MAGICCUP_SCRIPT_COMMON_SCRIPT_TILE9_FRAME_H
#define MAGICCUP_SCRIPT_COMMON_SCRIPT_TILE9_FRAME_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>
#include <Helper/Type/vectori2.h>

namespace magiccup {

class ScriptTile9Frame final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptTile9Frame);

  void SetImageSize(opgs16::DVectorInt2 size);

  void SetEdgeImageSize(int32_t pixel_value);

  void SetInterval(int32_t length);

  void SetAlpha(float alpha);

  void Execute(bool is_openup);

  bool IsExecutionDone() const noexcept;

private:
  void Initiate() override final;

  void Update(float delta_time) override final;

  opgs16::element::canvas::CImage* m_object = nullptr;
  opgs16::component::CSprite2DRenderer* m_renderer = nullptr;
  opgs16::element::CShaderWrapper* m_renderer_wrapper = nullptr;

  opgs16::element::CTimerHandle m_timer_handle;

  opgs16::DVectorInt2 m_edge_size   = {};
  opgs16::DVectorInt2 m_frame_size  = {};

  int32_t m_effect_interval = 0;
  int32_t m_elapsed = 0;

  bool m_uniform_dirty  = false;
  bool m_is_executing   = false;
  bool m_is_openup = false;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_COMMON_SCRIPT_TILE9_FRAME_H