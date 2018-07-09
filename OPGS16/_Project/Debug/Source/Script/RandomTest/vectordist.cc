
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RandomTest/vectordist.h"

#include <Component/procedural2d_renderer.h>
#include <Element/object.h>

namespace opgs16 {
  struct DColor32;
  struct DVectorInt2;
}

namespace opgs16 {
  namespace component {
    struct DTexelInfo;
  }
}

namespace debug::script {

void VectorDistribution::Start() {
  using opgs16::component::DTexelInfo;
  using opgs16::DVectorInt2;
  using opgs16::DColor32;

  auto& object = GetBindObject();
  renderer = object.GetComponent<opgs16::component::CProcedural2DRenderer>();

  renderer->SetSize(m_size * 2, m_size * 2);
  std::vector<DTexelInfo> info;
  const int32_t iter = 360;
  const float radian = static_cast<float>(360) / iter * 3.1415926535f / 180;

  float present_rad = 0.f;
  info.reserve(iter);
  for (int32_t i = 0; i < iter; ++i) {
    const auto index = GetIndexVector({cosf(present_rad), sinf(present_rad)});
    info.emplace_back(DTexelInfo{ index , DColor32::DarkGray });
    present_rad += radian;
  }
  renderer->UpdateBuffer(info);

  object.SetWorldPosition({128.f, 128.f, 0.f});
  object.SetScaleValue(static_cast<float>(m_size));
}

void VectorDistribution::InsertVector(const opgs16::DVector2& vector) {
  using opgs16::component::DTexelInfo;
  using opgs16::DColor32;
  renderer->UpdateBuffer(DTexelInfo{ GetIndexVector(vector), DColor32::Yellow });
}

opgs16::DVectorInt2
VectorDistribution::GetIndexVector(const opgs16::DVector2& vector) {
  opgs16::DVectorInt2 result;
  result.x = static_cast<int32_t>((vector.x * 0.5f * 0.75f + 0.5f) * m_size * 2);
  result.y = static_cast<int32_t>((vector.y * 0.5f * 0.75f + 0.5f) * m_size * 2);

  return result;
}

} /// ::debug::script namespace