#ifndef OPGS16_ELEMENT_INTERNAL_MODEL_OBJECT_H
#define OPGS16_ELEMENT_INTERNAL_MODEL_OBJECT_H

///
/// @file Element/Internal/model_object.h
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-12 Create file
///

#include <string>

#include <Phitos/Enums/success.h>

#include <Element/Internal/mesh_object.h>

struct aiNode;
struct aiScene;

namespace opgs16::element {

///
/// @enum EModelMeshType
///
/// @brief
///
///
enum class EModelMeshType {
  Single,
  Multiple
};

///
/// @class DModelObject
///
/// @brief
///
///
class DModelObject {
public:
  explicit DModelObject() = default;
  explicit DModelObject(std::string& model_path);

  virtual ~DModelObject() = default;

  virtual std::string GetModelName() const noexcept;

protected:
  ///
  /// @brief
  ///
  /// @param[in] node
  /// @param[in] scene
  ///
  void ProcessNode(const aiNode& node, const aiScene& scene);

  // Each model directly stores mesh object information (not initiated yet)
  std::vector<DMeshObject> m_meshes = {};

  EModelMeshType m_model_mesh_type = EModelMeshType::Single;

  std::string m_model_name = "";
  std::string m_directory_path = "";
};

} /// ::opgs16::element

#endif /// OPGS16_ELEMENT_INTERNAL_MODEL_OBJECT_H