#include "scene.h"
#include "..\Debugs\hierarchy_tree.h"

void Scene::Update() {
	for (auto& object : m_object_list) {
		if (object.second->GetActive())
			object.second->Update();
	}
}

void Scene::Draw() {
	for (auto& object : m_object_list) { object.second->Draw(); }
}

void Scene::GetObjectTree(ObjectTree* const tree) {
	for (const auto& object : m_object_list) {
		ObjectTree child; child.name = object.first;
		tree->children.push_back(std::move(child));
		object.second->GetObjectTree(&*tree->children.rbegin());
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
