
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/scene.cc
///
/// @brief Implementation file of opgs16::element::CScene
///
/// @author Jongmin Yun
///
/// @log
/// 2018-04-14 Add comments. Move definitions into ::opgs16::element namespace.
/// 2018-04-14 Move file to /System/Element/Public/ directory.
///

/// Header file
#include <Element/scene.h>

namespace opgs16::element {

void CScene::Update(float delta_time) {
	for (auto& object : m_object_list) {
		if (object.second->GetActive())
			object.second->Update(delta_time);
	}
}

CScene::object_map& CScene::GetObjectList() {
	return m_object_list;
}

CScene::object_ptr& CScene::GetGameObject(const std::string& name) {
	auto it = m_object_list.find(name);
	if (it != m_object_list.end()) {
		return (*it).second;
	}
}

void CScene::ScriptInitiate(object_ptr& object_pointer) {
  auto list = object_pointer->GetComponents<component::CScriptFrame>();
  unsigned index = 0;
  for (const auto script_ptr : list) {
    script_ptr->Initiate();
    PUSH_LOG_INFO_EXT(
        "Object call Initiate() : [Name : {0}] [Id : {1}]",
        object_pointer->GetObjectName(), index);
    ++index;
  }
}

} /// ::opgs16::element namespace
