#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Builtin/model/model_3dsphere.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-12 Create file
///

/// Header file
#include <Element/Builtin/Model/model_3dsphere.h>
#include <Helper/Math/math.h>

namespace opgs16::builtin::model {

BModel3DSphere::BModel3DSphere() {
  m_model_mesh_type = element::EModelMeshType::Single;
  m_directory_path = "";

  // Make mesh
  element::DMeshObject mesh;

  // Vertices
  mesh.ActivateVertices();
  std::vector<element::DMeshVector> vertices;
  const auto a = DVector3{ 0, 1, 0 };
  const auto b = DVector3{ 0, 0, 1 };
  const auto c = DVector3{ 1, 0, 0 };
  const auto d = DVector3{-1,-1,-1 }.Normalize();

  const int32_t level = 4;
  pCreateVertexRecursively(a, b, c, level, vertices);
  pCreateVertexRecursively(b, c, d, level, vertices);
  pCreateVertexRecursively(c, d, a, level, vertices);
  pCreateVertexRecursively(d, a, b, level, vertices);

  mesh.VerticesReserve(static_cast<int32_t>(vertices.size()));
  for (const auto& vertex : vertices) {
    mesh.PushVertice(vertex);
  }

  mesh.ActivateMeshObject();
  m_meshes.emplace_back(std::move(mesh));
}

void BModel3DSphere::pEmplaceVertex(const DVector3& normalized_position,
                                    std::vector<element::DMeshVector>& container) {
  container.emplace_back(normalized_position, normalized_position, DVector3{}, DVector2{});
}

void BModel3DSphere::pCreateVertexRecursively(const DVector3& a, const DVector3& b,
                                              const DVector3& c, int32_t level,
                                              std::vector<element::DMeshVector>& container) {
  if (level <= 0) {
    pEmplaceVertex(a, container);
    pEmplaceVertex(b, container);
    pEmplaceVertex(c, container);
    return;
  }

  const auto ab = ((a + b) / 2.f).Normalize();
  const auto bc = ((b + c) / 2.f).Normalize();
  const auto ca = ((c + a) / 2.f).Normalize();
  pCreateVertexRecursively(a, ab, ca, level - 1, container);
  pCreateVertexRecursively(ab, b, bc, level - 1, container);
  pCreateVertexRecursively(c, ca, bc, level - 1, container);
  pCreateVertexRecursively(ab, bc, ca, level - 1, container);
}

} /// ::opgs16::builtin::model namespace