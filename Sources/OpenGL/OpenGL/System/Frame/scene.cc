#include "scene.h"

Scene::Scene() : camera{ camera::GetCamera() } {
    camera.SetPerspective(50.f, 360.f / 240.f, 0.1f, 100.f);
}

void Scene::HandleInput(GLFWwindow* const window) {
    float camera_speed = 0.025f;

    if (DoesKeyPressed(window, GLFW_KEY_W))
        camera.MovePosition(camera::Movement::UP, camera_speed);
    else if (DoesKeyPressed(window, GLFW_KEY_S))
        camera.MovePosition(camera::Movement::DOWN, camera_speed);
    else if (DoesKeyPressed(window, GLFW_KEY_A))
        camera.MovePosition(camera::Movement::LEFT, camera_speed);
    else if (DoesKeyPressed(window, GLFW_KEY_D))
        camera.MovePosition(camera::Movement::RIGHT, camera_speed);
    else if (DoesKeyPressed(window, GLFW_KEY_E)) // Zoom in
        camera.Zoom(camera::ZoomMode::IN);
    else if (DoesKeyPressed(window, GLFW_KEY_Q))
        camera.Zoom(camera::ZoomMode::OUT);
}

void Scene::Update() {
	for (auto& object : objects) { object.second->Update(); }
}

void Scene::Draw() {
	for (auto& object : objects) { object.second->Draw(); }
}

auto Scene::GetObjects() -> std::unordered_map<std::string, std::unique_ptr<Object>>& {
	return objects;
}

std::unique_ptr<Object>& Scene::GetObject(const std::string && tag) {
	auto it = objects.find(tag);
	if (it != objects.end()) {
		return (*it).second;
	}
}
