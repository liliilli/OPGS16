#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/mesh_manager.h
///
/// @brief
/// Internal management file of meshes. 2d, 3d.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-10 Create file.
///

/// Header file
#include <Manager/mesh_manager.h>

#include <unordered_map>

#include <Phitos/Dbg/assert.h>
#include <Phitos/Enums/initiated.h>

/// ::opgs16::element::DModelObject
#include <Element/Internal/model_object.h>
/// ::opgs16::element::DMeshObject
#include <Element/Internal/mesh_object.h>
#include <Element/Internal/vao_container.h>

#include <Element/Default/model_2dquad.h>
#include <Element/Builtin/Model/model_2dquadline.h>
#include <Element/Builtin/Model/model_3dbox.h>
#include <Element/Builtin/Model/model_3dsphere.h>
#include <Element/Builtin/Model/model_point.h>
#include "Element/Builtin/Model/model_empty.h"

//!
//! Data and flags
//!

namespace {
using phitos::enums::EInitiated;
using opgs16::element::_internal::EVboBufferType;
using opgs16::element::_internal::EEboBufferType;

EInitiated b_initiated = EInitiated::NotInitiated;
EInitiated b_builtin_initiated = EInitiated::NotInitiated;

using ModelMap = std::unordered_map<std::string, opgs16::element::DModelObject>;
ModelMap m_model_map = {};

} /// unnamed namespace

//!
//! Global static functions.
//!

namespace {

///
/// @brief
///
///
void InitiateBuiltinModelMeshes() {
  PHITOS_ASSERT(b_initiated == EInitiated::NotInitiated,
      "Built-in mesh initiation must be called in Initiate() funciton.");
  PHITOS_ASSERT(b_builtin_initiated == EInitiated::NotInitiated,
      "Duplicated built-in model initiation must be called only once.");

  using opgs16::builtin::model::BModel2DQuad;
  using opgs16::builtin::model::BModel2DQuadLine;
  using opgs16::builtin::model::BModelPoint;
  using opgs16::builtin::model::BModel3DBox;
  using opgs16::builtin::model::BModel3DSphere;
  using opgs16::builtin::model::BModelEmpty;

  m_model_map.try_emplace(BModelEmpty::s_model_name, BModelEmpty{});
  m_model_map.try_emplace(opgs16::builtin::g_model_2d_quad, BModel2DQuad{});
  m_model_map.try_emplace(opgs16::builtin::g_model_2d_quad_dynamic, BModel2DQuad{});
  m_model_map.try_emplace(BModel2DQuadLine::s_model_name, BModel2DQuadLine{});
  m_model_map.try_emplace(BModelPoint::s_model_name, BModelPoint{});
  m_model_map.try_emplace(BModel3DBox::s_model_name, BModel3DBox{});
  m_model_map.try_emplace(BModel3DSphere::s_model_name, BModel3DSphere{});

  b_builtin_initiated = EInitiated::Initiated;
}

}

//!
//! Definitions
//!

namespace opgs16::manager::mesh {

void Initiate() {
  PHITOS_ASSERT(b_initiated == EInitiated::NotInitiated,
      "Duplicated mesh manager initiation is prohibited.");

  InitiateBuiltinModelMeshes();

  b_initiated = EInitiated::Initiated;
}

void Shutdown() {
  m_model_map.clear();

  b_initiated = EInitiated::NotInitiated;
}

phitos::enums::EFound IsModelExist(const std::string& model_name) {
  return (m_model_map.find(model_name) != m_model_map.end()) ?
          phitos::enums::EFound::Found :
          phitos::enums::EFound::NotFound;
}

phitos::enums::ESucceed GenerateModel(const std::string& resource_model_name) {
  PHITOS_NOT_IMPLEMENTED_ASSERT();

  return phitos::enums::ESucceed::Succeed;
}

std::pair<std::unique_ptr<element::CVaoContainer>, phitos::enums::ESucceed>
GenerateVaoItemsFromModel(const std::string& model_name) {
  using element::_internal::EVboBufferType;
  using element::_internal::EEboBufferType;

  return GenerateVaoItemsFromModelExt(model_name,
                                      EVboBufferType::StaticDraw,
                                      EEboBufferType::StaticDraw);
}


std::pair<std::unique_ptr<element::CVaoContainer>, phitos::enums::ESucceed>
GenerateVaoItemsFromModelExt(const std::string& model_name, EVboBufferType vbo_type, EEboBufferType ebo_type) {
  using phitos::enums::EActivated;
  using element::_internal::CInternalVertexArrayObject;
  using element::_internal::EVboBufferType;
  using element::_internal::EEboBufferType;
  using element::_internal::EBufferTarget;
  using phitos::type::PtTByte;

  const auto& meshes = m_model_map[model_name].GetMeshes();

  // Create vao resource lists.
  std::vector<CInternalVertexArrayObject> vao_list;
  vao_list.reserve(meshes.size());

  for (const auto& mesh : meshes) {
    if (mesh.IsVerticeActivated() == EActivated::Disabled) continue;

    // Check this mesh have vertices.
    const auto vbo_byte_length = PtTByte{mesh.GetByteSizeOfVertices()};

    // Check this mesh have indices.
    const auto is_indice_activated = mesh.IsIndiceActivated();
    const auto ebo_byte_length = PtTByte{mesh.GetByteSizeOfIndices()};

    if (is_indice_activated == EActivated::Activated) {
      vao_list.emplace_back(vbo_type, PtTByte{vbo_byte_length}, mesh.GetVerticesSize(),
                            ebo_type, PtTByte{ebo_byte_length}, mesh.GetIndicesSize());
    }
    else {
      vao_list.emplace_back(vbo_type, PtTByte{vbo_byte_length}, mesh.GetVerticesSize());
    }

    auto last_vao_item = vao_list.rbegin();
    last_vao_item->Map(EBufferTarget::VertexBuffer, 0_pByte, vbo_byte_length, mesh.GetVerticesData());

    if (is_indice_activated == EActivated::Activated) {
      last_vao_item->Map(EBufferTarget::ElementBuffer, 0_pByte, ebo_byte_length, mesh.GetIndiceData());
    }
  }

  return {std::make_unique<element::CVaoContainer>(model_name, std::move(vao_list)),
          phitos::enums::ESucceed::Succeed};
}

} /// ::opgs16::manager::mesh namespace