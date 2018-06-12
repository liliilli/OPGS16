
///
/// @file Element/Internal/model_object.cc
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-12 Create file
///

/// Header file
#include <Element/Internal/model_object.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <Phitos/Dbg/assert.h>
#include <Phitos/Enums/activated.h>

#include <Element/Internal/mesh_object.h>
#include <Headers/import_logger.h>

//!
//! Local functions
//!

namespace {

///
/// @brief
///
/// @param[in] mat
/// @param[in] type
///
/// @return
///
std::vector<opgs16::element::DMeshTextureMetaInfo>
LoadMaterialTextures(const aiMaterial& mat, opgs16::element::EMeshTextureType type) {
  std::vector<opgs16::element::DMeshTextureMetaInfo> textures;

  aiTextureType ai_type = aiTextureType_NONE;
  switch (type) {
  case opgs16::element::EMeshTextureType::DiffuseMap:
    ai_type = aiTextureType_DIFFUSE;
    break;
  case opgs16::element::EMeshTextureType::SpecularMap:
    ai_type = aiTextureType_SPECULAR;
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

  const auto texture_count = static_cast<uint32_t>(mat.GetTextureCount(ai_type));

  for (uint32_t i = 0; i < texture_count; ++i) {
    aiString string;
    mat.GetTexture(ai_type, i, &string);
    textures.emplace_back(string.C_Str(), type);
  }

  return textures;
}

///
/// @brief
///
///
/// @param[in] mesh
/// @param[in] i
///
/// @return
///
///
opgs16::element::DMeshVector GenerateMeshVector(const aiMesh& mesh, uint32_t i) noexcept {
  opgs16::DVector3 position = {};
  opgs16::DVector3 normal = {};
  opgs16::DVector3 tangent = {};
  opgs16::DVector2 texcoord = {};

  using phitos::enums::EActivated;
  EActivated m_is_position_activated = EActivated::Disabled;
  EActivated m_is_normal_activated   = EActivated::Disabled;
  EActivated m_is_tangent_activated  = EActivated::Disabled;
  EActivated m_is_texcoord_activated = EActivated::Disabled;

  if (mesh.HasPositions()) {
    position = mesh.mVertices[i];
    m_is_position_activated = EActivated::Activated;
  }

  if (mesh.HasNormals()) {
    normal = mesh.mNormals[i];
    m_is_normal_activated = EActivated::Activated;
  }

  if (mesh.HasTangentsAndBitangents()) {
    tangent = mesh.mTangents[i];
    m_is_tangent_activated = EActivated::Activated;
  }

  if (mesh.HasTextureCoords(0)) {
    texcoord = mesh.mTextureCoords[0][i];
    m_is_texcoord_activated = EActivated::Activated;
  }

  return {
      {position, m_is_position_activated},
      {normal, m_is_normal_activated},
      {tangent, m_is_tangent_activated},
      {texcoord, m_is_texcoord_activated}
  };
}

///
/// @brief
/// Generate mesh information which is consisted of position, normal, tangent,
/// material meta-information, and indices etc.
///
/// @param[in] mesh
/// @param[in] scene
///
/// @return
///
///
opgs16::element::DMeshObject GenerateMesh(aiMesh& mesh,
                                          const aiScene& scene) {
  using phitos::enums::EActivated;
  opgs16::element::DMeshObject mesh_object;

  // Process vertex position, normals and texture coordinates
  if (const auto num_vertices = static_cast<uint32_t>(mesh.mNumVertices);
      num_vertices > 0) {
    mesh_object.ActivateVertices();
    mesh_object.VerticesReserve(num_vertices);

    for (uint32_t i = 0; i < num_vertices; ++i)
      mesh_object.PushVertice(GenerateMeshVector(mesh, i));
  }

  // Process indices
  if (mesh.HasFaces()) {
    mesh_object.ActivateIndices();
    const auto num_faces = static_cast<uint32_t>(mesh.mNumFaces);

    for (uint32_t i = 0; i < num_faces; ++i) {
      const aiFace& face = mesh.mFaces[i];

      for (uint32_t j = 0, num_indices = face.mNumIndices; j < num_indices; ++j) {
        mesh_object.PushIndice(face.mIndices[j]);
      }
    }
  }

  // Process materials
  if (mesh.mMaterialIndex >= 0) {
    mesh_object.ActivateMaterials();

    const aiMaterial& material = *scene.mMaterials[mesh.mMaterialIndex];
    auto meta_diffuse_maps = LoadMaterialTextures(
        material, opgs16::element::EMeshTextureType::DiffuseMap);
    auto meta_specular_maps = LoadMaterialTextures(
        material, opgs16::element::EMeshTextureType::SpecularMap);
  }

  mesh_object.ActivateMeshObject();
  return mesh_object;
}

}

//!
//! Definition
//!

namespace opgs16::element {

phitos::enums::ESucceed DModelObject::GenerateModel(std::string& model_path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
      model_path.c_str(),
      aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    PUSH_LOG_ERROR_EXT("Could not load model file : {}, "
        "Change it to alternative builtin model.", model_path);
    return phitos::enums::ESucceed::Failed;
  }

  m_directory_path = model_path.substr(0, model_path.find_last_of('/'));

  if (scene->mRootNode->mNumMeshes > 1)
    m_model_mesh_type = EModelMeshType::Multiple;
  else
    m_model_mesh_type = EModelMeshType::Single;

  // Process node
  ProcessNode(*scene->mRootNode, *scene);
  return phitos::enums::ESucceed::Succeed;
}

void DModelObject::ProcessNode(const aiNode& node, const aiScene& scene) {
  const auto mesh_size = static_cast<uint32_t>(node.mNumMeshes);
  for (uint32_t i = 0; i < mesh_size; ++i) {
    aiMesh* mesh = scene.mMeshes[node.mMeshes[i]];
    m_meshes.emplace_back(GenerateMesh(*mesh, scene));
  }

  // Recursively generate nodes.
  const auto children_size = static_cast<uint32_t>(node.mNumChildren);
  for (uint32_t i = 0; i < children_size; ++i) {
    ProcessNode(*node.mChildren[i], scene);
  }
}

} /// ::opgs16::element namespace