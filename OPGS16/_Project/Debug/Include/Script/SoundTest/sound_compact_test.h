#ifndef OPGS16_SYSTEM_BOOT_SCRIPTS____3_PUBLIC_CANVAS_SCRIPT_H
#define OPGS16_SYSTEM_BOOT_SCRIPTS____3_PUBLIC_CANVAS_SCRIPT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// ::opgs16::component::CScriptFrame */
#include <Component\script_frame.h>
/// Forward declaration
#include <opgs16fwd.h>

namespace debug::script {

class SoundCompactTest final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(SoundCompactTest);

private:
  void Initiate() override final;
  void Start() override final;
  void Update(float delta_time) override final;
  void Destroy() override final;

  void Input();

  opgs16::element::canvas::CText* m_bgm_sentence = nullptr;
  opgs16::element::canvas::CText* m_info_sentence = nullptr;

  bool    m_is_play_bgm = false;
  int16_t m_mode = 0;
};

} /// ::debug::script namespace

#endif /// OPGS16_SYSTEM_BOOT_SCRIPTS____3_PUBLIC_CANVAS_SCRIPT_H