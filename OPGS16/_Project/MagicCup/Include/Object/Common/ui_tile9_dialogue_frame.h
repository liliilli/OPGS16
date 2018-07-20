#ifndef MAGICCUP_OBJECT_COMMON_UI_TILE9_DIALOGUE_FRAME_H
#define MAGICCUP_OBJECT_COMMON_UI_TILE9_DIALOGUE_FRAME_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/image.h>

//!
//! Forward declaration
//!

namespace opgs16 {
struct DVectorInt2;
}

namespace magiccup {
class ScriptTile9Frame;
}

//!
//! Implementation
//!

namespace magiccup {

class UiTile9DialogueFrame final : public opgs16::element::canvas::CImage {
public:
  UiTile9DialogueFrame(const std::string& sprite_tag,
                       const opgs16::element::canvas::CCanvas* ref_canvas);

  void SetFullImageSize(const opgs16::DVectorInt2& size_vector) noexcept;

  void SetEdgeImageSize(int32_t pixel_value) noexcept;

  void SetInterval(int32_t length) noexcept;

  void SetAlpha(float alpha) noexcept;

  void Execute(bool is_openup) noexcept;

  ScriptTile9Frame* GetScript() const;

  inline static constexpr const char* s_shader_name = "Tile9FrameShader";

private:
  ScriptTile9Frame* m_script = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_COMMON_UI_TILE9_DIALOGUE_FRAME_H