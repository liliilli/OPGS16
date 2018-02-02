#include "ui_object.h"

void UiObject::Update() {
	/** Update children */
	for (auto& child : GetChildren()) {
		auto child_temp = std::static_pointer_cast<UiObject>(child.second);
		child_temp->UpdateFinalPosition(screen_x, screen_y, screen_width, screen_height);
		child_temp->Update();
	}
}

void UiObject::UpdateFinalPosition(
	const float parent_x,
	const float parent_y,
	const float parent_width,
	const float parent_height) {
	/** Body */
	auto origin	= static_cast<int>(GetOrigin()) - 1;
	unsigned y = origin / 3;
	unsigned x = origin % 3;

	GLint source_x = static_cast<GLint>(parent_x + (parent_width / 2) * x);
	GLint source_y = static_cast<GLint>(parent_y + (parent_height / 2) * y);

	SetFinalPosition({ GetLocalPosition() + glm::vec3{source_x, source_y, 0} });
	for (auto& child : GetChildren()) {
		/** TODO :: NEED PERFORMANCE CHECK */
		auto child_temp = std::static_pointer_cast<UiObject>(child.second);
		child_temp->UpdateFinalPosition(screen_x, screen_y, screen_width, screen_height);
	}
}

void UiObject::Draw() {
	for (const auto& child : GetChildren()) {
		child.second->Draw();
	}
}

void UiObject::UpdateScreenXYWH(const std::array<GLint, 4> xywh) {
	screen_x		= static_cast<float>(xywh[0]);
	screen_y		= static_cast<float>(xywh[1]);
	screen_width	= static_cast<float>(xywh[2]);
	screen_height	= static_cast<float>(xywh[3]);
}
