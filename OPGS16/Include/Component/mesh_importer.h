#ifndef OPGS16_COMPONENT_MESH_IMPORTER_H
#define OPGS16_COMPONENT_MESH_IMPORTER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/mesh_importer.h
///
/// @brief
/// Bring mesh to game object, being able to be useable paramater to mesh renderers.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-12 Create file.
///

#include <Component/Internal/component.h>
#include <Element/Internal/vao_container.h>

//!
//! Forward declaration
//!

namespace opgs16::component {
class CMeshRenderer;
};

//!
//! Implementation
//!

namespace opgs16::component {

class CMeshImporter final : public _internal::CComponent {
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CMeshImporter)
public:
  CMeshImporter(element::CObject& bind_object, const std::string& mesh_name);
  ~CMeshImporter();

  CMeshImporter(const CMeshImporter&) = default;
  CMeshImporter(CMeshImporter&&) = default;
  CMeshImporter& operator=(const CMeshImporter&) = default;
  CMeshImporter& operator=(CMeshImporter&&) = default;

private:
  void Update(float delta_time) override final;

  ///
  /// @brief Get vao resource's raw poitner reference.
  /// This function will not increase/decrease reference count.
  ///
  element::CVaoContainer* pGetVaoRawRef() noexcept;

  ///
  element::CVaoContainer* m_vao_ref = nullptr;

  friend class opgs16::component::CMeshRenderer;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_MESH_IMPORTER_H