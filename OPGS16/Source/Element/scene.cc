
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

void CScene::Destroy() {
  if (m_main_camera)
    m_main_camera = nullptr;
  m_object_list.clear();
  m_name_counter.clear();
}

void CScene::Update(float delta_time) {
  using phitos::enums::EActivated;

	for (auto& object : m_object_list) {
		if (object.second)
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

} /// ::opgs16::element namespace
