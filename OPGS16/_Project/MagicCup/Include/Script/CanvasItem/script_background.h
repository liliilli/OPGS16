#ifndef MAGICCUP_SCRIPT_CANVASITEM_SCRIPT_BACKGROUND_H
#define MAGICCUP_SCRIPT_CANVASITEM_SCRIPT_BACKGROUND_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {

class ScriptUiBackground final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptUiBackground);

  void SetTile(int32_t tile_index);

  void SetOffset(const float x, const float y) noexcept;

  void SetScale(const float x, const float y) noexcept;

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  opgs16::element::CShaderWrapper* m_wrapper = nullptr;

  float m_elapsed = 0.0f;
  float m_mov_x = 0.f;
  float m_mov_y = 0.f;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_CANVASITEM_SCRIPT_BACKGROUND_H