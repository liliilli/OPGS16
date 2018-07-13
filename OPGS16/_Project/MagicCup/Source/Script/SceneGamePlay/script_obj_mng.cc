
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_obj_mng.h"

#include "../../../Include/Object/SceneGamePlay/Cup/obj_cup.h"
#include "../../../Include/Object/SceneGamePlay/Cup/obj_cup_item.h"
#include "../../../Include/Script/SceneGamePlay/Cup/script_cup_number.h"

namespace magiccup {

void ScriptObjectManagement::Initiate() {
  auto& obj = GetBindObject();

  const int32_t number = 5;

  float interval = static_cast<float>((256 - (64)) / (number - 1));
  opgs16::DVector3 point = {-(256 - 64) / 2, 0.f, 0.f};

  for (int32_t i = 1; i <= number; ++i) {
    auto item = obj.Instantiate<ObjectCupItem>(ObjectCupItem::s_object_name);
    item->SetWorldPosition(point);
    point += opgs16::DVector3{interval, 0.f, 0.f};

    auto script = item->
        GetChild(ObjectCup::s_object_name)->
        GetComponent<ScriptCupNumbering>();
    if (script) {
      script->SetNumber(i);
    }
  }
}

} /// ::magiccup namespace