#include <precompiled.h>
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
#include <Phitos/Dbg/assert.h>

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

CScene::TGameObjectMap* CScene::GetGameObjectList() noexcept {
	return &m_object_list;
}

CObject* CScene::GetGameObject(const std::string& object_name,
                               bool is_resursive) {
  if (!is_resursive) {
    if (const auto it = m_object_list.find(object_name);
        it != m_object_list.end()) {
      return it->second.get();
    }
    return nullptr;
  }

  return GetGameObjectResursively(object_name);
}

CObject* CScene::GetGameObjectResursively(const std::string& object_name) noexcept {
  for (auto& element : m_object_list) {
    const auto& object = element.second;
    // If object is empty, pass it.
    if (object) continue;

    if (object->GetGameObjectName() == object_name) {
      return object.get();
    }

    if (const auto object_pointer = object->GetGameObject(object_name, true)) {
      return object_pointer;
    }
  }

  return nullptr;
}

} /// ::opgs16::element namespace
