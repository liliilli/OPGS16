#include <precompiled.h>

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Builtin/model/model_2dquadline.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-30 Create file.
///

/// Header file
#include <Element/Builtin/Model/model_2dquadline.h>

namespace opgs16::builtin::model {

BModel2DQuadLine::BModel2DQuadLine() {
  m_model_mesh_type = element::EModelMeshType::Single;
  m_directory_path = "";

  element::DMeshObject mesh;
  mesh.ActivateVertices();
  mesh.VerticesReserve(4);
  std::vector<element::DMeshVector> vertices;
  vertices.reserve(4);
  vertices.emplace_back(DVector3{1.f, 1.f, 0.f},
                        DVector3{0.f, 0.f, 1.f},
                        DVector3{}, DVector2{});

  vertices.emplace_back(DVector3{1.f,-1.f, 0.f},
                        DVector3{0.f, 0.f, 1.f},
                        DVector3{}, DVector2{});

  vertices.emplace_back(DVector3{-1.f,-1.f, 0.f},
                        DVector3{0.f, 0.f, 1.f},
                        DVector3{}, DVector2{});

  vertices.emplace_back(DVector3{-1.f, 1.f, 0.f},
                        DVector3{0.f, 0.f, 1.f},
                        DVector3{}, DVector2{});
  for (const auto& vertex : vertices) {
    mesh.PushVertice(vertex);
  }

  mesh.ActivateMeshObject();
  m_meshes.emplace_back(std::move(mesh));
}

} /// ::opgs16::builtin::model namespace