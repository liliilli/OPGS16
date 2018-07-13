#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_CUP_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_CUP_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace magiccup {
class ScriptCupNumbering;
}

namespace magiccup {

class ObjectCup final : public opgs16::element::CObject {
public:
  ObjectCup();

  inline static constexpr const char* s_object_name = "Cup";

private:
  void Render() override final;

  opgs16::component::CSprite2DRenderer* m_cup_texture = nullptr;
  opgs16::element::CShaderWrapper* m_wrapper = nullptr;
  ScriptCupNumbering* m_numbering_script = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_CUP_H