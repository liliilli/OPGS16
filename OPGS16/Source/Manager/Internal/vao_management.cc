
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
///

#include <Manager/Internal/vao_management.h>

#include <unordered_map>

#include <Phitos/Dbg/assert.h>
#include <Phitos/Enums/initiated.h>
#include <Phitos/Types/ptTByte.h>

/// ::opgs16::element::_internal::CInternalVertexArrayObject
#include <Element/Internal/internal_vertex_array_object.h>

#include <Headers/import_logger.h>


//!
//! Member & flags
//!

namespace {
constexpr float test_quad[18] = {
  -1.0f, 1.0f, 0.f,
  -1.0f, -1.0f, 0.f,
  1.0f, 1.0f, 0.f,
  -1.0f, -1.0f, 0.f,
  1.0f, -1.0f, 0.f,
  1.0f, 1.0f, 0.f
};

using EInitiated = phitos::enums::EInitiated;
using EFound = phitos::enums::EFound;
using TVaoMapContainer =
    std::unordered_map<
        std::string,
        std::shared_ptr<opgs16::element::_internal::CInternalVertexArrayObject>>;

EInitiated m_initiated = EInitiated::NotInitiated;

TVaoMapContainer m_common_vao_container;
TVaoMapContainer m_builtin_vao_container;

} /// unnamed namespace.

namespace {

///
/// @brief
///
///
void InitiateBuiltinVaoItems() {
  using EVboBufferType = opgs16::element::_internal::EVboBufferType;
  using EBufferTarget = opgs16::element::_internal::EBufferTarget;
  using CInternalVertexArrayObject =
      opgs16::element::_internal::CInternalVertexArrayObject;

  auto [it, result] = m_builtin_vao_container.try_emplace("Error",
      std::make_unique<CInternalVertexArrayObject>(
          "Error", EVboBufferType::StaticDraw, 72_pByte)
  );
  if (!result) {
    PHITOS_ASSERT(result == true,
        "Builtin Vertex Array Object is not created properly.");
    return;
  }
  it->second->Map(EBufferTarget::VertexBuffer,
                  0_pByte, 72_pByte, (void*)test_quad);
}

///
/// @brief
///
inline EFound IsInternalVaoExist(const std::string& vao_name) {
  return m_common_vao_container.find(vao_name) != m_common_vao_container.end() ?
         EFound::Found :
         EFound::NotFound;
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

void Deactivate() {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      "Can not deactivate resource without calling Initiated() function.");

  m_common_vao_container.clear();
  m_builtin_vao_container.clear();

  m_initiated = EInitiated::NotInitiated;
}

std::pair<
    std::weak_ptr<element::_internal::CInternalVertexArrayObject>,
    phitos::enums::EFound>
GetVaoResource(const std::string& vao_name) {
  if (IsInternalVaoExist(vao_name) == EFound::NotFound) {
    return {m_builtin_vao_container["Error"], EFound::NotFound};
  }

  return {m_common_vao_container[vao_name], EFound::Found};
}

#ifdef false
std::pair<
    std::weak_ptr<element::_internal::CInternalVertexArrayObject>,
    phitos::enums::ESucceed>
SetVaoResource(const std::string& vao_name,
               element::_internal::EVboBufferType vbo_buffer_type,
               phitos::type::PtTByte vbo_buffer_size) {
  using CInternalVertexArrayObject =
      element::_internal::CInternalVertexArrayObject;

  auto instance = std::make_unique<CInternalVertexArrayObject>(
      vao_name.c_str(), vbo_buffer_type, vbo_buffer_size);
}
#endif

} /// ::opgs16::manager::_internal::vao namespace.