#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_NUMBER_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_NUMBER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace magiccup {

class ObjectNumber final : public opgs16::element::CObject {
public:
  ObjectNumber();

  inline static constexpr const char* s_object_name = "Numb";

private:
  void Render() override final;

  opgs16::component::CSprite2DRenderer* m_texture = nullptr;
  opgs16::element::CShaderWrapper* m_wrapper = nullptr;
};

}

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_NUMBER_H