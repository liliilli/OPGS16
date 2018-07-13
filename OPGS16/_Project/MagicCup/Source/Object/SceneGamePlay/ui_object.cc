
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/ui_object.h"

#include "../../../Include/Object/SceneGamePlay/ui_life.h"
#include "../../../Include/Object/SceneGamePlay/ui_score.h"
#include "../../../Include/Object/SceneGamePlay/ui_timelimit.h"
#include "../../../Include/Object/SceneGamePlay/ui_stage.h"

namespace magiccup {

GamePlayUi::GamePlayUi() {
  SetScaleFactor({128.f, 112.f, 0});

  Instantiate<UiScore>(UiScore::s_object_name);
  Instantiate<UiLife>(UiLife::s_object_name);
  Instantiate<UiTimelimit>(UiTimelimit::s_object_name);
  Instantiate<UiStage>(UiStage::s_object_name);
}

void GamePlayUi::LocalUpdate() {
	const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

  UiObject::LocalUpdate();
}

} /// ::magiccup namespace