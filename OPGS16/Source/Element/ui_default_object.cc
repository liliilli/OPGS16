#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
///
/// @file Element/default_uiobject.cc
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-18 Create file
///

/// Header file
#include <Element/ui_default_object.h>

namespace opgs16::element {

void CUiDefaultObject::LocalUpdate() {
  const auto wh = GetLocalScale() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };

	UpdateScreenXYWH(xywh);
	UiObject::LocalUpdate();
}

} /// ::opgs16::element namespace