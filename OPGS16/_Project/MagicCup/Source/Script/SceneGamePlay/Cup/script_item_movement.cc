
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Cup/script_item_movement.h"

#include <Element/object.h>

#include "../../../../Include/Object/SceneGamePlay/Cup/obj_ball.h"
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_cup.h"
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_shadow.h"
#include "Component/sprite2d_renderer.h"

namespace magiccup {

void ScriptItemMovement::Initiate() {
  auto& obj = GetBindObject();

  obj.CreateGameObject<ObjectCupShadow>(ObjectCupShadow::s_object_name);
  obj.CreateGameObject<ObjectBall>(ObjectBall::s_object_name);

  auto cup = obj.CreateGameObject<ObjectCup>(ObjectCup::s_object_name);
  cup->SetWorldPosition({0.f, ObjectCup::s_initial_y_pos, 0.f});

  SetComponentActive(false);
}

void ScriptItemMovement::DownRenderingLayer() {
  auto& obj = GetBindObject();

  auto nm = obj.GetGameObject(ObjectCupShadow::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(2);

  nm = obj.GetGameObject(ObjectBall::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(2);

  nm = obj.GetGameObject(ObjectCup::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(3);
}

void ScriptItemMovement::UpRenderingLayer() {
  auto& obj = GetBindObject();

  auto nm = obj.GetGameObject(ObjectCupShadow::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(0);

  nm = obj.GetGameObject(ObjectBall::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(0);

  nm = obj.GetGameObject(ObjectCup::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(1);
}

void ScriptItemMovement::RevertRenderingLayer() {
  auto& obj = GetBindObject();

  auto nm = obj.GetGameObject(ObjectCupShadow::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(1);

  nm = obj.GetGameObject(ObjectBall::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(1);

  nm = obj.GetGameObject(ObjectCup::s_object_name);
  nm->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetRenderingLayer(2);
}

void ScriptItemMovement::Update(float delta_time) {
  elapsed += (delta_time * 1000) / interval;

  if (!m_function) {
    PUSH_LOG_ERRO("ScriptItemMovement Moving function is not bind.");
    return;
  }

  auto& obj = GetBindObject();
  obj.SetWorldPosition(m_function(start_point, destination_point, elapsed));

  if (elapsed >= 1.0f) {
    obj.SetWorldPosition(destination_point);
    RevertRenderingLayer();
    SetComponentActive(false);
    elapsed = 0.f;
  }
}

void ScriptItemMovement::SetMoveFunction(std::function<TCallback> function) {
  m_function = function;
}

void ScriptItemMovement::SetMoveSetting(opgs16::DVector3 destination,
                                        int32_t interval) {
  start_point = GetBindObject().GetWorldPosition();
  destination_point = destination;
  this->interval = interval;
  elapsed = 0.f;
}

void ScriptItemMovement::ExecuteMoving() {
  SetComponentActive(true);
}

} /// ::magiccup namespace