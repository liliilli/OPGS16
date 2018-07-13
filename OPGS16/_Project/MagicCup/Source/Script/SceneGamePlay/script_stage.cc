
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_stage.h"

#include <Element/Canvas/text.h>

namespace magiccup {

void ScriptUiStage::Initiate() {
  using opgs16::element::canvas::CText;
  auto& obj = GetBindObject();

  auto text = obj.Instantiate<CText>("Text", "Stage 01");
  text->SetAlignment(IAlignable::Alignment::CENTER);
  text->SetFontName("opSystem");
  text->SetFontSize(8);
}

} /// ::magiccup namespace