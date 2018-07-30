#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Default/model_2dquad.cc
///
/// @brief
/// Built-in 2d quad model class definition file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-13 Create file
///

/// Header file
#include <Element/Default/model_2dquad.h>

#include <array>

namespace opgs16::builtin::model {

BModel2DQuad::BModel2DQuad() {
  m_model_mesh_type = element::EModelMeshType::Single;
  m_directory_path = "";

  // Make mesh
  element::DMeshObject mesh;

  // Vertices
  mesh.ActivateVertices();
  mesh.VerticesReserve(4);
  std::vector<element::DMeshVector> vertices;
  vertices.emplace_back(DVector3{1.f, 1.f, 0.f},
                        DVector3{0.f, 0.f, 1.f},
                        DVector3{},
                        DVector2{1.f, 1.f});

  vertices.emplace_back(DVector3{1.f,-1.f, 0.f},
                        DVector3{0.f, 0.f, 1.f},
                        DVector3{},
                        DVector2{1.f, 0.f});

  vertices.emplace_back(DVector3{-1.f,-1.f, 0.f},
                        DVector3{0.f, 0.f, 1.f},
                        DVector3{},
                        DVector2{0.f, 0.f});

  vertices.emplace_back(DVector3{-1.f, 1.f, 0.f},
                        DVector3{0.f, 0.f, 1.f},
                        DVector3{},
                        DVector2{0.f, 1.f});
  for (const auto& vertex : vertices)
    mesh.PushVertice(vertex);

  // Indices
  mesh.ActivateIndices();
  std::array<uint32_t, 6> indices = {0, 1, 2, 2, 3, 0};
  for (const auto index : indices)
    mesh.PushIndice(index);

  mesh.ActivateMeshObject();
  m_meshes.emplace_back(std::move(mesh));
}

} /// ::opgs16::builtin::model namespace