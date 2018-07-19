#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_SHADOW_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_SHADOW_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace magiccup {

class ObjectCupShadow final : public opgs16::element::CObject {
public:
  ObjectCupShadow();

  inline static constexpr const char* s_object_name = "Shadow";
private:
  void Render() override final;

  opgs16::component::CSprite2DRenderer* m_texture = nullptr;
  opgs16::element::CShaderWrapper*      m_wrapper = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_SHADOW_H