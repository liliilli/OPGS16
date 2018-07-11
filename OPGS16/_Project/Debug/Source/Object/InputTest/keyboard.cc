
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/InputTest/keyboard.h"

#include "../../../Include/Internal/key_code.h"
#include "../../../Include/Object/InputTest/key.h"

namespace debug::object {

KeyboardUiObject::KeyboardUiObject(opgs16::element::canvas::CCanvas* canvas) {
  using keyword::EKeyboardKeyCode;

  this->SetOrigin(Origin::UP_LEFT);

  const auto start  = static_cast<int32_t>(EKeyboardKeyCode::A);
  const auto end    = static_cast<int32_t>(EKeyboardKeyCode::Shift);
  for (int32_t i = start; i <= end; ++i) {
    auto key = Instantiate<object::KeyImage>(
        "_", static_cast<EKeyboardKeyCode>(i), canvas);
  }
}

void KeyboardUiObject::LocalUpdate() {
  const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	UiObject::LocalUpdate();
}

} /// ::debug::object namespace
