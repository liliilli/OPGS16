
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RenderingTest/procedural_obj.h"

#include <Component/procedural2d_renderer.h>
#include <Element/object.h>

namespace debug::script {

void ProceduralObject::Start() {
  using opgs16::component::DTexelInfo;
  using opgs16::DVectorInt2;
  using opgs16::DColor32;

  auto& object = GetBindObject();
  auto renderer = object.GetComponent<opgs16::component::CProcedural2DRenderer>();

  const int32_t size = 64;
  renderer->SetSize(size * 2, size * 2);
  std::vector<DTexelInfo> info;
  const int32_t iter = 360;
  const float radian = static_cast<float>(360) / iter * 3.1415926535f / 180;

  float present_rad = 0.f;
  info.reserve(iter);
  for (int32_t i = 0; i < iter; ++i) {
    const float unit_x = cosf(present_rad) * 0.5f;
    const float unit_y = sinf(present_rad) * 0.5f;
    present_rad += radian;

    const auto index_x = static_cast<int32_t>((unit_x * 0.75f + 0.5f) * size * 2);
    const auto index_y = static_cast<int32_t>((unit_y * 0.75f + 0.5f) * size * 2);
    info.emplace_back(DTexelInfo{ DVectorInt2{index_x, index_y}, DColor32::White });
  }
  renderer->UpdateBuffer(info);

  object.SetWorldPosition({128.f, 128.f, 0.f});
  object.SetScaleValue(size);
}

} /// ::debug::script namespace