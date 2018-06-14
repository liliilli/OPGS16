#ifndef OPGS16_ELEMENT_DEFAULT_MODEL_2DQUAD_H
#define OPGS16_ELEMENT_DEFAULT_MODEL_2DQUAD_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Default/model_2dquad.h
///
/// @brief
/// Built-in 2d quad model class.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-13 Create file.
///

#include <string_view>

/// ::opgs16::builtin builtin vao names.
#include <Element/Default/builtin_vao_name.h>
/// ::opgs16::element::DModelObject
#include <Element/Internal/model_object.h>

namespace opgs16::builtin::model {

///
/// @class BModel2DQuad
///
/// @brief
/// Bulit-in instance which creates x, y coordinate 2d quad model
/// with ebo.
///
class BModel2DQuad final : public opgs16::element::DModelObject {
public:
  BModel2DQuad();

  std::string GetModelName() const noexcept override final {
    return m_model_name.data();
  }

public:
  static constexpr std::string_view m_model_name = g_model_2d_quad;
};

} /// ::opgs16::builtin::model namespace

#endif /// OPGS16_ELEMENT_DEFAULT_MODEL_2DQUAD_H