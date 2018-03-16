#include "scene.h"

void Scene::Update() {
	for (auto& object : m_object_list) {
		if (object.second->GetActive())
			object.second->Update();
	}
}

Scene::object_map& Scene::GetObjectList() {
	return m_object_list;
}

Scene::object_ptr& Scene::GetObject(const std::string& name) {
	auto it = m_object_list.find(name);
	if (it != m_object_list.end()) {
		return (*it).second;
	}
}
