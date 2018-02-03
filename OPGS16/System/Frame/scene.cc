#include "scene.h"

void Scene::HandleInput(GLFWwindow* const window) {
    float camera_speed = 0.025f;

    //if (DoesKeyPressed(window, GLFW_KEY_W))
    //    camera.MovePosition(camera::Movement::UP, camera_speed);
    //else if (DoesKeyPressed(window, GLFW_KEY_S))
    //    camera.MovePosition(camera::Movement::DOWN, camera_speed);
    //else if (DoesKeyPressed(window, GLFW_KEY_A))
    //    camera.MovePosition(camera::Movement::LEFT, camera_speed);
    //else if (DoesKeyPressed(window, GLFW_KEY_D))
    //    camera.MovePosition(camera::Movement::RIGHT, camera_speed);
    //else if (DoesKeyPressed(window, GLFW_KEY_E)) // Zoom in
    //    camera.Zoom(camera::ZoomMode::IN);
    //else if (DoesKeyPressed(window, GLFW_KEY_Q))
    //    camera.Zoom(camera::ZoomMode::OUT);
}

void Scene::Update() {
	for (auto& object : objects) {
		if (object.second->GetActiveValue())
			object.second->Update();
	}
}

void Scene::Draw() {
	for (auto& object : objects) { object.second->Draw(); }
}

void Scene::GetObjectTree(ObjectTree* const tree) {
	for (const auto& object : objects) {
		ObjectTree child; child.name = object.first;
		tree->children.push_back(std::move(child));
		object.second->GetObjectTree(&*tree->children.rbegin());
	}
}

auto Scene::GetObjects() -> std::unordered_map<std::string, std::shared_ptr<Object>>& {
	return objects;
}

std::shared_ptr<Object>& Scene::GetObject(const std::string && tag) {
	auto it = objects.find(tag);
	if (it != objects.end()) {
		return (*it).second;
	}
}
