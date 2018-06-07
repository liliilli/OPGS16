
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
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

/// opgs16::builtin::shader::SGlobalQuad2D
#include <Shader/Default/shader_quad2d.h>
/// opgs16::builtin::shader::SGlobalFont
#include <Shader/Default/shader_font.h>
/// opgs16::builtin::shader::SPostProcessPlainQuad
#include <Shader/Default/shader_postprocess_plain_quad.h>

/// import logger
#include <Headers/import_logger.h>

//!
//! Forward declaration.
//!

///
/// @brief Override version method of CreateShader(std::string, shader_list).
/// @param[in] tag The tag to specify.
/// @param[in] container vector conatiner contains shader arguments to create.
/// @return created shader raw-pointer.
///
opgs16::manager::shader::shader_raw
    CreateShader(const std::string& tag,
                 const opgs16::resource::SShader& container);

///
/// @brief
/// Check and return true / false whether or not Shader with name is already initiated.
///
/// @param[in] shader_name Shader name to search.
///
/// @return If shader already exist, return true else false.
///
inline bool DoesShaderExist(const std::string& shader_name);

//!
//! Data
//!

namespace {
// Shader container.
opgs16::manager::shader::shader_map m_shaders{};
} /// unnamed namespace

//!
//! Definitions.
//!

namespace opgs16::manager::shader {

void Initiate() {
  using namespace builtin::shader;

  m_shaders[SGlobalQuad2D::s_shader_name] = std::make_unique<SGlobalQuad2D>();
  m_shaders[SGlobalQuad2D::s_shader_name]->Link();
  m_shaders[SGlobalFont2D::s_shader_name] = std::make_unique<SGlobalFont2D>();
  m_shaders[SGlobalFont2D::s_shader_name]->Link();
  m_shaders[SGlobalPostProcessingQuad::s_shader_name] =
    std::make_unique<SGlobalPostProcessingQuad>();
  m_shaders[SGlobalPostProcessingQuad::s_shader_name]->Link();

}

void ReleaseAll() {
}

void ReleaseShader(const std::string& shader_name) {
  if (DoesShaderExist(shader_name))
    m_shaders.erase(shader_name);
}

shader_raw GetShader(const std::string& name) {
  if (!DoesShaderExist(name)) {
		auto& list = manager::resource::GetShader(name);
		CreateShader(name, list);
	}
	return m_shaders[name].get();
}

} /// ::opgs16::manager::shader namespace.

opgs16::manager::shader::shader_raw CreateShader(
    const std::string& tag,
    const opgs16::resource::SShader& container) {
	if (DoesShaderExist(tag))
    return opgs16::manager::shader::GetShader(tag);

	auto shader = std::make_unique<opgs16::element::CShaderNew>();
	for (auto& [type, shader_path] : container.List()) {
		shader->SetShader(type, shader_path.c_str());
	}
	shader->Link();

	// Bind
	m_shaders[tag] = std::move(shader);
	return m_shaders[tag].get();
}

inline bool DoesShaderExist(const std::string& shader_name) {
  return m_shaders.find(shader_name) != m_shaders.end();
}
