#ifndef OPGS16_MANAGER_INTERNAL_SHADER_BUILTIN_KEYWORDS_H
#define OPGS16_MANAGER_INTERNAL_SHADER_BUILTIN_KEYWORDS_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <array>

#include <Manager/ResourceType/shader.h>

namespace opgs16::builtin {

inline static constexpr const char* s_uniform_position = "opPosition";
inline static constexpr const char* s_uniform_normal = "opNormal";
inline static constexpr const char* s_uniform_tangent = "opTangent";
inline static constexpr const char* s_uniform_texcoord = "opTexcoord";
inline static constexpr const char* s_uniform_boneid = "opBoneId";
inline static constexpr const char* s_uniform_weight = "opWeight";

inline static constexpr const char* s_uniform_proj = "opProj";
inline static constexpr const char* s_uniform_view = "opView";
inline static constexpr const char* s_uniform_model = "opModel";
inline static constexpr const char* s_uniform_projview = "opProjView";

inline static constexpr const char* s_uniform_texelld = "opTexelLD";
inline static constexpr const char* s_uniform_texelru = "opTexelRU";
inline static constexpr const char* s_uniform_scale = "opScale";
inline static constexpr const char* s_uniform_offset = "opOffset";

inline static constexpr const char* s_uniform_alpha = "opAlpha";
inline static constexpr const char* s_uniform_color = "opColor";
inline static constexpr const char* s_uniform_texture0 = "opTexture0";

inline static constexpr const char* s_shader_builtin[17] = {
  s_uniform_position,
  s_uniform_normal,
  s_uniform_tangent,
  s_uniform_texcoord,
  s_uniform_boneid,
  s_uniform_weight,
  s_uniform_proj,
  s_uniform_view,
  s_uniform_model,
  s_uniform_projview,
  s_uniform_texelld,
  s_uniform_texelru,
  s_uniform_scale,
  s_uniform_offset,
  s_uniform_alpha,
  s_uniform_color,
  s_uniform_texture0
};

inline static constexpr const opgs16::resource::SShader::EVariableType
s_shader_builtin_type[17] = {
  opgs16::resource::SShader::EVariableType::Vec3,
  opgs16::resource::SShader::EVariableType::Vec3,
  opgs16::resource::SShader::EVariableType::Vec3,
  opgs16::resource::SShader::EVariableType::Vec2,
  opgs16::resource::SShader::EVariableType::IVec3,
  opgs16::resource::SShader::EVariableType::Vec3,
  opgs16::resource::SShader::EVariableType::Matrix4,
  opgs16::resource::SShader::EVariableType::Matrix4,
  opgs16::resource::SShader::EVariableType::Matrix4,
  opgs16::resource::SShader::EVariableType::Matrix4,
  opgs16::resource::SShader::EVariableType::Vec2,
  opgs16::resource::SShader::EVariableType::Vec2,
  opgs16::resource::SShader::EVariableType::Vec2,
  opgs16::resource::SShader::EVariableType::Vec2,
  opgs16::resource::SShader::EVariableType::Float,
  opgs16::resource::SShader::EVariableType::Vec4,
  opgs16::resource::SShader::EVariableType::Texture2D
};

inline static constexpr int32_t builtin_size = 17;

} /// ::opgs16

#endif /// OPGS16_MANAGER_INTERNAL_SHADER_BUILTIN_KEYWORDS_H