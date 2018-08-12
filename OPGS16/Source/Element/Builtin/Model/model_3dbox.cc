#include <precompiled.h>

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Builtin/model/model_3dbox.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-11 Create file
///

/// Header file
#include <Element/Builtin/Model/model_3dbox.h>

#define NEW_VERTEX(__MAInstance__, __MAPosition__, __MANormal__, __MATangent__, __MATexcoord__) \
  __MAInstance__.emplace_back(DVector##__MAPosition__, DVector3##__MANormal__, DVector3##__MATangent__, DVector3##__MATexcoord__)

namespace opgs16::builtin::model {

BModel3DBox::BModel3DBox() {
  m_model_mesh_type = element::EModelMeshType::Single;
  m_directory_path = "";

  // Make mesh
  element::DMeshObject mesh;

  // Vertices
  mesh.ActivateVertices();
  mesh.VerticesReserve(24);
  std::vector<element::DMeshVector> vertices;
  // FrontBack
  vertices.emplace_back(DVector3{  1,  1,  1 }, DVector3{ 0, 0, 1 }, DVector3{}, DVector2{ 1.f, 1.f });
  vertices.emplace_back(DVector3{  1, -1,  1 }, DVector3{ 0, 0, 1 }, DVector3{}, DVector2{ 1.f, 0.f });
  vertices.emplace_back(DVector3{ -1, -1,  1 }, DVector3{ 0, 0, 1 }, DVector3{}, DVector2{ 0.f, 0.f });
  vertices.emplace_back(DVector3{ -1,  1,  1 }, DVector3{ 0, 0, 1 }, DVector3{}, DVector2{ 0.f, 1.f });
  vertices.emplace_back(DVector3{ -1,  1, -1 }, DVector3{ 0, 0, -1 }, DVector3{}, DVector2{ 1.f, 1.f });
  vertices.emplace_back(DVector3{ -1, -1, -1 }, DVector3{ 0, 0, -1 }, DVector3{}, DVector2{ 1.f, 0.f });
  vertices.emplace_back(DVector3{  1, -1, -1 }, DVector3{ 0, 0, -1 }, DVector3{}, DVector2{ 0.f, 0.f });
  vertices.emplace_back(DVector3{  1,  1, -1 }, DVector3{ 0, 0, -1 }, DVector3{}, DVector2{ 0.f, 1.f });
  // UpDown
  vertices.emplace_back(DVector3{  1,  1, -1 }, DVector3{ 0, 1, 0 }, DVector3{}, DVector2{ 1.f, 1.f });
  vertices.emplace_back(DVector3{  1,  1,  1 }, DVector3{ 0, 1, 0 }, DVector3{}, DVector2{ 1.f, 0.f });
  vertices.emplace_back(DVector3{ -1,  1,  1 }, DVector3{ 0, 1, 0 }, DVector3{}, DVector2{ 0.f, 0.f });
  vertices.emplace_back(DVector3{ -1,  1, -1 }, DVector3{ 0, 1, 0 }, DVector3{}, DVector2{ 0.f, 1.f });
  vertices.emplace_back(DVector3{  1, -1,  1 }, DVector3{ 0, -1, 0 }, DVector3{}, DVector2{ 1.f, 1.f });
  vertices.emplace_back(DVector3{  1, -1, -1 }, DVector3{ 0, -1, 0 }, DVector3{}, DVector2{ 1.f, 0.f });
  vertices.emplace_back(DVector3{ -1, -1, -1 }, DVector3{ 0, -1, 0 }, DVector3{}, DVector2{ 0.f, 0.f });
  vertices.emplace_back(DVector3{ -1, -1,  1 }, DVector3{ 0, -1, 0 }, DVector3{}, DVector2{ 0.f, 1.f });
  // LeftRight
  vertices.emplace_back(DVector3{  1,  1, -1 }, DVector3{ 1, 0, 0 }, DVector3{}, DVector2{ 1.f, 1.f });
  vertices.emplace_back(DVector3{  1, -1, -1 }, DVector3{ 1, 0, 0 }, DVector3{}, DVector2{ 1.f, 0.f });
  vertices.emplace_back(DVector3{  1, -1,  1 }, DVector3{ 1, 0, 0 }, DVector3{}, DVector2{ 0.f, 0.f });
  vertices.emplace_back(DVector3{  1,  1,  1 }, DVector3{ 1, 0, 0 }, DVector3{}, DVector2{ 0.f, 1.f });
  vertices.emplace_back(DVector3{ -1,  1,  1 }, DVector3{ -1, 0, 0 }, DVector3{}, DVector2{ 1.f, 1.f });
  vertices.emplace_back(DVector3{ -1,  1, -1 }, DVector3{ -1, 0, 0 }, DVector3{}, DVector2{ 1.f, 0.f });
  vertices.emplace_back(DVector3{ -1, -1, -1 }, DVector3{ -1, 0, 0 }, DVector3{}, DVector2{ 0.f, 0.f });
  vertices.emplace_back(DVector3{ -1, -1,  1 }, DVector3{ -1, 0, 0 }, DVector3{}, DVector2{ 0.f, 1.f });

  for (const auto& vertex : vertices) {
    mesh.PushVertice(vertex);
  }

  mesh.ActivateIndices();
  std::array<uint32_t, 36> indices = {
    0,  3, 1, 1, 3, 2,
    4,  7, 5, 5, 7, 6,
    8, 11, 9, 9,11,10,
    12,15,13,13,15,14,
    16,19,17,17,19,18,
    20,23,21,21,23,22
  };
  for (const auto index : indices)
    mesh.PushIndice(index);

  mesh.ActivateMeshObject();
  m_meshes.emplace_back(std::move(mesh));
}

} /// ::opgs16::builtin::model namespace

