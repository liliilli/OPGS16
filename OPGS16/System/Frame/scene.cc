#include "scene.h"
#include "..\Debugs\hierarchy_tree.h"

void Scene::Update() {
	for (auto& object : objects) {
		if (object.second->GetActive())
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

Object::object_map& Scene::GetObjects() {
	return objects;
}

Object::object_ptr& Scene::GetObject(const std::string& tag) {
	auto it = objects.find(tag);
	if (it != objects.end()) {
		return (*it).second;
	}
}
