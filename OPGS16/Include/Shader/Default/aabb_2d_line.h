#ifndef OPGS16_SHADER_DEFAULT_AABB_2D_LINE_H
#define OPGS16_SHADER_DEFAULT_AABB_2D_LINE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Shader/Default/aabb_2d_line.h
///
/// @brief
/// AABB 2d box rendering shader program class file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-07 Create file.
///

/// ::opgs16::element::CShaderNew
#include <Shader/shader.h>

namespace opgs16::builtin::shader {

class SAABB2DShader final : public element::CShaderNew {
public:
  SAABB2DShader();
  inline static const char* s_shader_name = "__gAabb2d";
};

} /// ::opgs16::builtin::shader namespace

#endif /// OPGS16_SHADER_DEFAULT_AABB_2D_LINE_H