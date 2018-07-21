#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_CUP_SCRIPT_ITEM_MOVEMENT_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_CUP_SCRIPT_ITEM_MOVEMENT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <functional>

#include <Component/script_frame.h>

#include <Helper/Type/vector3.h>

namespace magiccup {

class ScriptItemMovement final : public opgs16::component::CScriptFrame {
  using TCallback = opgs16::DVector3(opgs16::DVector3, opgs16::DVector3, float);

public:
  OP16_SCRIPT_GENERATE(ScriptItemMovement);

  void SetMoveFunction(std::function<TCallback> function);

  void SetMoveSetting(opgs16::DVector3 destination, int32_t interval);

  void ExecuteMoving();

  void DownRenderingLayer();

  void UpRenderingLayer();

  void RevertRenderingLayer();

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  std::function<TCallback> m_function;

  opgs16::DVector3 start_point;
  opgs16::DVector3 destination_point;
  int32_t interval = 0;
  float elapsed = 0.f;


};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_CUP_SCRIPT_ITEM_MOVEMENT_H