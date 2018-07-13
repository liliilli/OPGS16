
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/ui_score.h"

#include "../../../Include/Script/SceneGamePlay/script_score.h"

namespace magiccup {

UiScore::UiScore() {
  SetOrigin(Origin::UP_LEFT);
  AddComponent<magiccup::ScriptUiScore>(*this);
}

  void UiScore::LocalUpdate() {
  const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

  UiObject::LocalUpdate();
}

} /// ::magiccup namespace