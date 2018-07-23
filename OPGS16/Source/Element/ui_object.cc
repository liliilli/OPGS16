#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/ui_object.cc

/// @log
/// 2018-03-11 Add boilerplate comment, replace it within ::opgs16::element.
/// 2018-07-02 Refactring.
///

/// Header file
#include <Element/ui_object.h>

#include <array>

#include <glm/glm.hpp>

namespace opgs16::element {

void UiObject::LocalUpdate() {
  using phitos::enums::EActivated;
  for (auto& child : m_children) {
    if (child.second &&
        child.second->IsObjectActive() == EActivated::Activated) {
      auto child_temp = static_cast<UiObject*>(child.second.get());
      child_temp->SetUiParentPosition(screen_origin.x, screen_origin.y,
                                      screen_size.x, screen_size.y);
    }
  }
}

void UiObject::SetUiParentPosition(
    const float parent_x,
    const float parent_y,
    const float parent_width,
    const float parent_height) {
  const auto origin	= static_cast<int>(GetOrigin()) - 1;
	const int32_t x = origin % 3;
	const int32_t y = origin / 3;

	const auto source_x = static_cast<int32_t>(parent_x + (parent_width / 2) * x);
	const auto source_y = static_cast<int32_t>(parent_y + (parent_height / 2) * y);

  SetParentPosition( DVector3{
      static_cast<float>(source_x),
      static_cast<float>(source_y),
      0.f }
  );

	for (auto& child : GetGameObjectList()) {
		/** TODO :: NEED PERFORMANCE CHECK */
		auto child_temp = static_cast<UiObject*>(child.second.get());
    child_temp->SetUiParentPosition(screen_origin.x, screen_origin.y,
                                    screen_size.x, screen_size.y);
	}
}

void UiObject::UpdateScreenXYWH(const std::array<GLint, 4> xywh) {
	screen_origin.x = static_cast<float>(xywh[0]);
	screen_origin.y = static_cast<float>(xywh[1]);
	screen_size.x   = static_cast<float>(xywh[2]);
	screen_size.y   = static_cast<float>(xywh[3]);
}

} /// ::opgs16::element namespace
