#ifndef OPGS16_MANAGER_RESOURCETYPE_SHADER_H
#define OPGS16_MANAGER_RESOURCETYPE_SHADER_H

#include <list>
#include <string_view>
#include <unordered_map>

#include <opgs16fwd.h>

namespace opgs16::resource {

class SShader {
public:
  inline static constexpr const int32_t s_variable_size = 12;

  ///
  /// @brief Shader internal variable type for uniform and textures.
  ///
  enum class EVariableType {
    Matrix4 = 0, Matrix3,
    Vec4, Vec3, Vec2,
    Float,
    IVec4, IVec3, IVec2,
    Int, IntPtr,
    Texture2D
  };

  inline static constexpr const std::string_view s_variable_str[12] = {
    "MAT4", "MAT3",
    "VEC4", "VEC3", "VEC2",
    "FLOAT",
    "IVEC4", "IVEC3", "IVEC2",
    "INT", "INTPTR",
    "TEXTURE2D"
  };

  using TShaderPair = std::pair<EShaderType, std::string>;
  using TShaderList = std::list<TShaderPair>;
  using TVariableList = std::unordered_map<std::string, EVariableType>;

private:
  TShaderList   m_shader_list;
  TVariableList  m_variable_list;

public:
  SShader() = default;
  explicit SShader(const TShaderList& shader_list,
                   const TVariableList& uniform_list = {}) :
      m_shader_list{ shader_list },
      m_variable_list{ uniform_list } {};

  inline const TShaderList& GetShaderList() const noexcept {
    return m_shader_list;
  }

  inline const TVariableList& GetVariableList() const noexcept {
    return m_variable_list;
  }
};

} /// ::opgs16::resource namespace

#endif /// OPGS16_MANAGER_RESOURCETYPE_SHADER_H
