
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Scene/ObjectTest/tree_activation.h"

#include "../../../Include/Object/Common/orthogonal_camera.h"
#include "../../../Include/Object/ObjectTest/node_manager.h"

#include "../../../Include/Object/Common/empty_canvas.h"
#include "../../../Include/Object/Common/subject.h"
#include "../../../Include/Object/Common/copyright.h"

#include "../../../Include/Script/ObjectTest/node_canvas_script.h"

namespace debug::scene {

void ObjectTreeActivation::Initiate() {
  Instantiate<object::OrthogonalCameraObject>("Camera");
  Instantiate<object::NodeManager>("Manager");

  auto canvas = Instantiate<object::EmptyCanvas>("Canvas");
  auto subject = canvas->Instantiate<object::Subject>("Subject");
  subject->SetText("Object Tree Act Test");

  canvas->Instantiate<object::Copyright>("Copyright");
  canvas->AddComponent<script::NodeCanvasScript>(*canvas);
}

} /// ::debug::scene namespace