
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/InputTest/key_log.h"

#include <Element/Canvas/text.h>

namespace debug::object {

KeyLogObject::KeyLogObject(opgs16::element::canvas::CCanvas* canvas) {
  using opgs16::element::canvas::CText;

  auto key_name = Instantiate<CText>("Name", "KeyboardKey");
  key_name->SetFontSize(8);
  key_name->SetWorldPosition({0, -4.f, 0});

  auto key_info = Instantiate<CText>("Info", "C:RPTX:IDL");
  key_info->SetFontSize(8);
  key_info->SetWorldPosition({0, -12.f, 0});
}

void KeyLogObject::LocalUpdate() {
  const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	UiObject::LocalUpdate();
}

} /// ::debug::object namespace