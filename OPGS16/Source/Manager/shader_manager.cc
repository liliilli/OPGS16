
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/shader_manager.cc
/// @brief ShaderManager implementation file.
///
/// @author Jongmin Yun
/// @log
/// 2018-03-04 Refactoring. Add copyright and comments.
/// 2018-04-03 Add default quad2d(gQuad) shader in bootstrapping.
///

/// Header file
#include <Manager/shader_manager.h>

#include <Phitos/Dbg/assert.h>
#include <Phitos/Enums/initiated.h>

/// ::opgs16::manager::resource_manager
#include <Manager/resource_manager.h>
/// ::opgs16::builtin::shader::SAABB2DShader
#include <Shader/Default/aabb_2d_line.h>
/// opgs16::builtin::shader::SGlobalQuad2D
#include <Shader/Default/shader_quad2d.h>
/// opgs16::builtin::shader::SGlobalFont
#include <Shader/Default/shader_font.h>
/// opgs16::builtin::shader::SPostProcessPlainQuad
#include <Shader/Default/shader_postprocess_plain_quad.h>
/// import logger
#include <Headers/import_logger.h>

//!
//! Data
//!

namespace {
using EInitiated = phitos::enums::EInitiated;
EInitiated s_initiated = EInitiated::NotInitiated;

opgs16::manager::shader::TShaderMap m_shader_container;
} /// unnamed namespace

namespace {

///
/// @brief
/// Check and return true / false whether or not Shader with name is already initiated.
///
/// @param[in] shader_name Shader name to search.
///
/// @return If shader already exist, return true else false.
///
inline bool IsShaderExist(const std::string& shader_name) noexcept {
  return m_shader_container.find(shader_name) != m_shader_container.end();
}

///
/// @brief Override version method of CreateShader(std::string, shader_list).
/// @param[in] shader_name The shader_name to specify.
/// @param[in] container vector conatiner contains shader arguments to create.
/// @return created shader raw-pointer.
///
opgs16::element::CShaderNew*
CreateShader(const std::string& shader_name,
             const opgs16::resource::SShader* container) {
	if (IsShaderExist(shader_name))
    return opgs16::manager::shader::GetShader(shader_name);

	auto shader = std::make_unique<opgs16::element::CShaderNew>();
  shader->SetShader(*container);

	auto [it, result] = m_shader_container.try_emplace(shader_name, std::move(shader));
  if (!result) {
    PUSH_LOG_WARN_EXT("Something was wrong on making shader, [Name : {}]", shader_name);
  }

  return it->second.get();
}

} /// unnamed namespace

//!
//! Definitions.
//!

namespace opgs16::manager::shader {

void Initiate() {
  PHITOS_ASSERT(s_initiated == EInitiated::NotInitiated,
      "Duplicated opgs16::manager::shader::Initiate() calling is prohibited.");
  using namespace builtin::shader;

  m_shader_container[SGlobalQuad2D::s_shader_name] = std::make_unique<SGlobalQuad2D>();
  m_shader_container[SGlobalFont2D::s_shader_name] = std::make_unique<SGlobalFont2D>();

  m_shader_container[SGlobalPostProcessingQuad::s_shader_name] =
    std::make_unique<SGlobalPostProcessingQuad>();

  auto [it, result] = m_shader_container.try_emplace(
      SAABB2DShader::s_shader_name,
      std::make_unique<SAABB2DShader>());
  PHITOS_ASSERT(result == true,
      "Invalid shader emplace, SAABB2DShader was not inserted properly. "
      "on opgs16::manager::shader::Initiate().");

  s_initiated = EInitiated::Initiated;
}

void ReleaseAll() {

}

void ReleaseShader(const std::string& shader_name) {
  if (IsShaderExist(shader_name))
    m_shader_container.erase(shader_name);
}

element::CShaderNew* GetShader(const std::string& shader_name) {
  if (!IsShaderExist(shader_name)) {
    const auto list = manager::resource::GetShader(shader_name);
    if (!list) {
      PHITOS_ASSERT(list != nullptr, "Could not find the shader you want.");
      return nullptr;
    }

		CreateShader(shader_name, list);
	}

	return m_shader_container[shader_name].get();
}

} /// ::opgs16::manager::shader namespace.
