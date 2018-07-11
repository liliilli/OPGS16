
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../../../Include/Object/InputTest/key.h"

#include <Element/Canvas/image.h>
#include <Helper/Type/vectori2.h>

namespace {

//!
//! EKeyboardKeyCode::A (16) is index 0.
//! EKeyboardKeyCode::Shift (73) is index 73 - 16.
//!
std::vector<opgs16::DVectorInt2> key_world_position = {
  {40, -80}, {112, -96}, {80, -96}, {72, -80}, {64, -64}, {88, -80}, {104, -80},
  {120, -80}, {144, -64}, {136, -80}, {152, -80}, {168, -80}, {144, -96}, {128, -96},
  {160, -64}, {176, -64}, {32, -64}, {80, -64}, {56, -80}, {96, -64}, {128, -64},
  {96, -96}, {48, -64}, {64, -96}, {112, -64}, {48, -96},

  {24, -48}, {40, -48}, {56, -48}, {72, -48}, {88, -48}, {104, -48},
  {120, -48}, {136, -48}, {152, -48}, {168, -48}, {184, -48}, {200, -48},

  {192, -64}, {208, -64}, {184, -80}, {200, -80}, {160, -96}, {176, -96}, {192, -96},

  {200, -120}, {216, -104}, {216, -120}, {232, -120},

  {8, -112}, {32, -112}, {8, -48}, {8, -32},
  {224, -64} /* Pipe */, {8, -64} /* Tab */, {224, -48} /*! Bs */,
  {216, -80} /*! Rt */, {8, -96}, /*! Shift */
};

enum EKeySizeType {
  One = 0,
  OneHalf,
  Double,
  Space
};

enum EKeyInputType {
  Idle,
  Press,
  Repeat,
  Released
};

std::vector<opgs16::DVectorInt2> key_type_size = { {16, 16}, {24, 16}, {32, 16} };
std::vector<std::array<int32_t, 4>> key_texture_index = {
  {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}
};

} /// unnamed namespace

namespace debug::object {

KeyImage::KeyImage(keyword::EKeyboardKeyCode key_code,
                   const opgs16::element::canvas::CCanvas* canvas_ref) {
  using opgs16::element::canvas::CImage;
  const auto key_val = static_cast<int32_t>(key_code);

  auto key_size = key_type_size[EKeySizeType::One];
  if (key_val >= 69 && key_val <= 71)
    key_size = key_type_size[EKeySizeType::OneHalf];
  else if (key_val > 71)
    key_size = key_type_size[EKeySizeType::Double];

  auto key = Instantiate<CImage>("Back", "Input", canvas_ref);
  key->SetImageSize(
      static_cast<float>(key_size.x),
      static_cast<float>(key_size.y));

  auto chr = Instantiate<CImage>("Front", "Input", canvas_ref);
  chr->SetImageSize(
      static_cast<float>(key_size.x),
      static_cast<float>(key_size.y));
  chr->SetTextureIndex(key_val);

  const auto index = key_val - 16;
  SetWorldPosition(
      static_cast<opgs16::DVector3>(key_world_position[index]) +
      opgs16::DVector3{
          static_cast<float>(key_size.x) / 2,
          -static_cast<float>(key_size.y) / 2,
          0.f
      }
  );
}

void KeyImage::LocalUpdate() {
  const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	UiObject::LocalUpdate();
}

} /// ::debug::object namespace