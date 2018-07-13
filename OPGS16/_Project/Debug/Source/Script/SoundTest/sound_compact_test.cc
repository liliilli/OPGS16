
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SoundTest/sound_compact_test.h"

#include <Element/object.h>
#include <Element/Canvas/canvas.h>
#include <Element/Canvas/text.h>
#include <Component/empty_renderer.h>
#include <Component/sprite_renderer.h>
#include <Manager/sound_manager.h>
#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Internal/keyword.h"
#include "../../../Include/Scene/select_scene.h"

namespace {

constexpr const char* k_instruction_name = "Instruction";
constexpr const char* k_instruction_string =
"Debug sound test\n"
"Key 1 : Drumloop BGM ON/OFF\n"
"Key 2 : Jaguar sound effect\n"
"Key 3 : c.ogg sound effect\n"
"Key 4 : Beep sound effect sqns\n";

constexpr const char* k_bgm_sentence_name = "BgmSentence";
constexpr const char* k_info_sentence_name = "InfoSentence";

} /// unnamed namespace

namespace debug::script {

void SoundCompactTest::Initiate() {
  using EOrigin = IOriginable::Origin;
  using EAlignment = IAlignable::Alignment;

  using CCanvas = opgs16::element::canvas::CCanvas;
  using CText = opgs16::element::canvas::CText;

  auto* obj = static_cast<CCanvas*>(&GetBindObject());

  // Make instruction text object.
  // This object will display debug test instruction.
  auto instruction = obj->Instantiate<CText>(k_instruction_name, k_instruction_string);
  instruction->SetOrigin(IOriginable::Origin::UP_LEFT);
  instruction->SetFontName("opSystem");
  instruction->SetFontSize(8u);
  instruction->SetWorldPosition(opgs16::DVector3{ 16, -24, 0 });
  instruction->SetColor(opgs16::DColor{ 1.f, 1.f, 1.f });

  // Bgm sentence object will display BGM ON/OFF status
  // depending on Background music play status on now.
  m_bgm_sentence = obj->Instantiate<CText>(k_bgm_sentence_name, "~BGM OFF~");
  m_bgm_sentence->SetOrigin(IOriginable::Origin::DOWN_CENTER);
  m_bgm_sentence->SetAlignment(IAlignable::Alignment::CENTER);
  m_bgm_sentence->SetFontName("opSystem");
  m_bgm_sentence->SetFontSize(8u);
  m_bgm_sentence->SetWorldPosition(opgs16::DVector3{ 0, 48, 0 });

  // Information sentence object will display what is playing now
  // depending on Key 2,3,4 mode.
  m_info_sentence = obj->Instantiate<CText>(k_info_sentence_name, "");
  m_info_sentence->SetOrigin(IOriginable::Origin::DOWN_CENTER);
  m_info_sentence->SetAlignment(IAlignable::Alignment::CENTER);
  m_info_sentence->SetFontName("opSystem");
  m_info_sentence->SetFontSize(8u);
  m_info_sentence->SetWorldPosition(opgs16::DVector3{ 0, 32, 0 });
}

void SoundCompactTest::Start() {
  using opgs16::manager::sound::GenerateSound;

  GenerateSound("Drumloop");
  GenerateSound("Jaguar");
  GenerateSound("c_ogg");
  GenerateSound("Beep1");
  GenerateSound("Beep2");
  GenerateSound("Beep3");
}

void SoundCompactTest::Destroy() {
  using opgs16::manager::sound::DestroySound;

  auto& bind_object = GetBindObject();
  bind_object.DestroyChild(k_instruction_name);
  bind_object.DestroyChild(*m_bgm_sentence);
  bind_object.DestroyChild(*m_info_sentence);

  DestroySound("Drumloop");
  DestroySound("Jaguar");
  DestroySound("c_ogg");
  DestroySound("Beep1");
  DestroySound("Beep2");
  DestroySound("Beep3");
}

void SoundCompactTest::Update(float delta_time) {
  using opgs16::manager::sound::IsSoundPlaying;
  using phitos::enums::EActivated;
  Input();

  switch (m_mode) {
  case 2:
    if (IsSoundPlaying("Jaguar") == EActivated::Disabled) {
      m_mode = 0;
      m_info_sentence->SetText("");
    }
    break;
  case 3:
    if (IsSoundPlaying("c_ogg") == EActivated::Disabled) {
      m_mode = 0;
      m_info_sentence->SetText("");
    }
    break;
  case 4:
    if (IsSoundPlaying("c_ogg") == EActivated::Disabled) {
      m_mode = 0;
      m_info_sentence->SetText("");
    }
    break;
  default: break;
  }
}

void SoundCompactTest::Input() {
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::sound::PlaySound;
  using opgs16::manager::sound::StopSound;

  if (IsKeyPressed("Key1")) {
    if (!m_is_play_bgm) {
      PlaySound("Drumloop");
      m_bgm_sentence->SetText("~BGM ON~");
      m_is_play_bgm = true;
    }
    else {
      StopSound("Drumloop");
      m_bgm_sentence->SetText("~BGM OFF~");
      m_is_play_bgm = false;
    }
  }
  else if (IsKeyPressed("Key2")) {
    PlaySound("Jaguar");
    m_info_sentence->SetText("Now playing Key2 Effect.");
    m_mode = 2;
  }
  else if (IsKeyPressed("Key3")) {
    PlaySound("c_ogg");
  }
  else if (IsKeyPressed("Key4")) {
    PlaySound("Beep1");
    PlaySound("Beep2");
    PlaySound("Beep3");
  }

  if (IsKeyPressed(keyword::key_backspace)) {
    M_REPLACE_SCENE(scene::SelectScene);
  }
}

} /// ::debug::script namespace