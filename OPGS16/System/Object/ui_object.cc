#include "ui_object.h"
#include <array>
#include <glm\glm.hpp>

void UiObject::LocalUpdate() {
    for (auto& child : m_children) {
        if (child.second && child.second->GetActive()) {
            auto child_temp = static_cast<UiObject*>(child.second.get());
            child_temp->SetUiParentPosition(screen_x, screen_y, screen_width, screen_height);
        }
    }
}

void UiObject::SetUiParentPosition(const float parent_x, const float parent_y,
                                   const float parent_width, const float parent_height) {
	auto origin	= static_cast<int>(GetOrigin()) - 1;
	unsigned x = origin % 3;
	unsigned y = origin / 3;

	GLint source_x = static_cast<GLint>(parent_x + (parent_width / 2) * x);
	GLint source_y = static_cast<GLint>(parent_y + (parent_height / 2) * y);

    SetParentPosition(glm::vec3{ source_x, source_y, 0 });
	for (auto& child : GetChildList()) {
		/** TODO :: NEED PERFORMANCE CHECK */
		auto child_temp = static_cast<UiObject*>(child.second.get());
		child_temp->SetUiParentPosition(screen_x, screen_y, screen_width, screen_height);
	}
}

void UiObject::UpdateScreenXYWH(const std::array<GLint, 4> xywh) {
	screen_x		= static_cast<float>(xywh[0]);
	screen_y		= static_cast<float>(xywh[1]);
	screen_width	= static_cast<float>(xywh[2]);
	screen_height	= static_cast<float>(xywh[3]);
}
