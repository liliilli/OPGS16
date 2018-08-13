#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Builtin/model/model_empty.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-13 Create file
///

/// Header file
#include <Element/Builtin/Model/model_empty.h>

namespace opgs16::builtin::model {

BModelEmpty::BModelEmpty() {
  m_model_mesh_type = element::EModelMeshType::Single;
  m_directory_path = "";

  element::DMeshObject mesh;
  mesh.ActivateVertices();
  mesh.VerticesReserve(1);

  // Make vertex information
  std::vector<element::DMeshVector> vertices;
  vertices.reserve(1);
  vertices.emplace_back(DVector3{}, DVector3{}, DVector3{}, DVector2{});

  // Insert information to mesh container.
  for (const auto& vertex : vertices) mesh.PushVertice(vertex);

  mesh.ActivateMeshObject();
  m_meshes.emplace_back(std::move(mesh));
}

} /// ::opgs16::builtin::model namespace