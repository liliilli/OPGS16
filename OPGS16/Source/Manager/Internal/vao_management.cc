#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/vao_management.cc
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-09 Create file.
/// 2018-06-14 Remove built-in vao container.
///

/// Header file
#include <Manager/Internal/vao_management.h>

#include <unordered_map>

#include <Phitos/Dbg/assert.h>
#include <Phitos/Enums/initiated.h>

/// ::opgs16::builtin::model::BModel2DQuad
#include <Element/Default/model_2dquad.h>
/// ::opgs16::element::_internal::CInternalVertexArrayObject
#include <Element/Internal/internal_vertex_array_object.h>
/// logger for debug mode.
#include <Headers/import_logger.h>
/// ::opgs16::manager::mesh
#include <Manager/mesh_manager.h>

//!
//! Member & flags
//!

namespace {
using EInitiated  = phitos::enums::EInitiated;
using EFound      = phitos::enums::EFound;
using TVaoMapContainer = std::unordered_map<
    std::string, std::unique_ptr<opgs16::element::CVaoContainer>
>;

EInitiated m_initiated = EInitiated::NotInitiated;
TVaoMapContainer m_vao_container;

} /// unnamed namespace.

//!
//! Function.
//!

namespace {

///
/// @brief /// Check whether or not vao called vao_name is exist in container.
/// @param[in] vao_name Vao name to find.
/// @return If find return EFound::Found or not.
///
inline EFound IsInternalVaoExist(const std::string& vao_name) {
  return m_vao_container.find(vao_name) != m_vao_container.end() ?
         EFound::Found :
         EFound::NotFound;
}

///
/// @brief
///
/// @param[in] model_name
///
/// @return The pair of
///
std::pair<opgs16::element::CVaoContainer*, phitos::enums::ESucceed>
GenerateVaoContainer(const std::string& model_name,
    opgs16::element::_internal::EVboBufferType vbo_type,
    opgs16::element::_internal::EEboBufferType ebo_type) {
  using phitos::enums::ESucceed;
  using opgs16::builtin::model::BModel2DQuad;
  using namespace opgs16::manager;

  if (IsInternalVaoExist(model_name) == EFound::Found)
    return {m_vao_container[model_name].get(), ESucceed::Failed};

  if (const auto result = mesh::IsModelExist(model_name);
      result == EFound::NotFound) {
    if (mesh::GenerateModel(model_name) == ESucceed::Failed) {
      PHITOS_UNEXPECTED_BRANCH();
      return {m_vao_container[BModel2DQuad::m_model_name.data()].get(),
              ESucceed::Failed};
    }
  }

  // Generate CVaoContainer unique_ptr on this.
  auto [instance_ptr, result] =
      mesh::GenerateVaoItemsFromModelExt(model_name, vbo_type, ebo_type);

  if (result == ESucceed::Failed) {
    PHITOS_ASSERT(result == ESucceed::Succeed,
        "Could not generate vao items from model.");
    return {m_vao_container[BModel2DQuad::m_model_name.data()].get(),
            ESucceed::Failed};
  }

  auto [it, insres] =
    m_vao_container.try_emplace(model_name, std::move(instance_ptr));
  if (!insres) PHITOS_UNEXPECTED_BRANCH();
  return {it->second.get(), ESucceed::Succeed};
}

///
/// @brief
/// Initiate built-in default vao items.
/// Built-in vao items are not destroyed until application ends.
///
void InitiateBuiltinVaoItems() {
  using EVboBufferType = opgs16::element::_internal::EVboBufferType;
  using EEboBufferType = opgs16::element::_internal::EEboBufferType;
  using EBufferTarget = opgs16::element::_internal::EBufferTarget;
  using CInternalVertexArrayObject =
      opgs16::element::_internal::CInternalVertexArrayObject;

  // 2D Quad (x, y) with element object buffer
  GenerateVaoContainer(
    opgs16::builtin::model::BModel2DQuad::m_model_name.data(),
    EVboBufferType::StaticDraw, EEboBufferType::StaticDraw
  );

  // 2D Quad (x, y) with element object buffer but only for dynamic like
  // font rendering.
  GenerateVaoContainer(
    opgs16::builtin::g_model_2d_quad_dynamic,
    EVboBufferType::DynamicDraw, EEboBufferType::StaticDraw
  );
}

}

//!
//! Implementation
//!

namespace opgs16::manager::_internal::vao {

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
      "Duplicated vao::Initiated() function call is prohibited.");

  // Generate builtin internal vao instances.
  InitiateBuiltinVaoItems();
  m_initiated = EInitiated::Initiated;
}

void Shutdown() {
  m_vao_container.clear();
  m_initiated = EInitiated::NotInitiated;
}

std::pair<element::CVaoContainer*, phitos::enums::EFound>
FindVaoResource(const std::string& vao_name) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      "Vao management was not initiated yet.");

  if (IsInternalVaoExist(vao_name) == EFound::NotFound) {
    return {m_vao_container["Error"].get(), EFound::NotFound};
  }

  return {m_vao_container[vao_name].get(), EFound::Found};
}

phitos::enums::EFound IsVaoResourceExist(const std::string& vao_name) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      "Vao management was not initiated yet.");
  return IsInternalVaoExist(vao_name);
}

std::pair<element::CVaoContainer*, phitos::enums::ESucceed>
GenerateVaoResourceWithModel(const std::string& model_name) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      "Vao management was not initiated yet.");
  return GenerateVaoContainer(model_name,
      element::_internal::EVboBufferType::StaticDraw,
      element::_internal::EEboBufferType::StaticDraw);
}

std::pair<element::CVaoContainer*, phitos::enums::ESucceed>
GenerateVaoResourceWithModelExt(
    const std::string& model_name,
    element::_internal::EVboBufferType vbo_type,
    element::_internal::EEboBufferType ebo_type) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      "Vao management was not initiated yet.");
  return GenerateVaoContainer(model_name, vbo_type, ebo_type);
}

} /// ::opgs16::manager::_internal::vao namespace.