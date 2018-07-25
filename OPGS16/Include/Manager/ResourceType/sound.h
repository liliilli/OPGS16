#ifndef OPGS16_MANAGER_RESOURCETYPE_SOUND_H
#define OPGS16_MANAGER_RESOURCETYPE_SOUND_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/ResourceType/sound.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-24 Create file, move resource sound type to this file.
///

#include <string>

namespace opgs16::resource {

enum class ESoundType {
  None,
  Effect,
  BackgroundMusic
};

class SSoundInfo {
private:
  std::string m_path = "";
  ESoundType  m_soundtype = ESoundType::None;

public:
  SSoundInfo() {};

  explicit SSoundInfo(const std::string& sound_file_path,
                      ESoundType sound_type) :
      m_path{ sound_file_path },
      m_soundtype{ sound_type }
  {};

  inline const std::string& GetPath() const noexcept {
    return m_path;
  }

  inline ESoundType GetSoundType() const noexcept {
    return m_soundtype;
  }
};

} /// ::resource namespace

#endif /// OPGS16_MANAGER_RESOURCETYPE_SOUND_H