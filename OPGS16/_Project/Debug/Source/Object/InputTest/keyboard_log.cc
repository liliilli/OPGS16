
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/InputTest/keyboard_log.h"

#include "../../../Include/Object/InputTest/key_log.h"

namespace debug::object {

KeyboardLogObject::KeyboardLogObject(opgs16::element::canvas::CCanvas* canvas) {
  SetOrigin(Origin::DOWN_LEFT);
  SetWorldPosition({8.f, 80.f, 0.f});

  Instantiate<KeyLogObject>("_", canvas);
  Instantiate<KeyLogObject>("_", canvas)->SetWorldPosition({80.f, 0.f, 0.f});
  Instantiate<KeyLogObject>("_", canvas)->SetWorldPosition({160.f, 0.f, 0.f});
  Instantiate<KeyLogObject>("_", canvas)->SetWorldPosition({0.f, -24.f, 0.f});
  Instantiate<KeyLogObject>("_", canvas)->SetWorldPosition({80.f, -24.f, 0.f});
  Instantiate<KeyLogObject>("_", canvas)->SetWorldPosition({160.f, -24.f, 0.f});
}

void KeyboardLogObject::LocalUpdate() {
  const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	UiObject::LocalUpdate();
}

} /// ::debug::object namespace