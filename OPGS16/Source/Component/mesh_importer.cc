#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/mesh_importer.cc
///
/// @brief
/// Bring mesh to game object, being able to be useable paramater to mesh renderers.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-12 Create file.
///

/// Header file
#include <Component/mesh_importer.h>
#include <Manager/Internal/vao_management.h>
#include <Phitos/Dbg/assert.h>

namespace opgs16::component {

CMeshImporter::CMeshImporter(element::CObject& bind_object, const std::string& mesh_name) :
    CComponent{ bind_object } {
  using phitos::enums::EFound;
  auto [ptr, result] = manager::_internal::vao::FindVaoResource(mesh_name);
  PHITOS_ASSERT(result == EFound::Found, "Failed to find vertex information object.");

  m_vao_ref = ptr;
  m_vao_ref->IncreaseCount();
  m_vao_ref->SetDirty();
}

CMeshImporter::~CMeshImporter() {
  m_vao_ref->DecreaseCount();
}

void CMeshImporter::Update(float delta_time) {}

element::CVaoContainer* CMeshImporter::pGetVaoRawRef() noexcept {
  return m_vao_ref;
}

} /// ::opgs16::component namespace