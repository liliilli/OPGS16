
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager\resource.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-02
/// Move ResourceManager class inside opgs16::manager namespace for uniformation.
/// Refactored and removed member functions are not appropriate for class.
/// 2018-03-03 Move member function to free funtion.
/// 2018-04-05 Get Texture2D to store atlas information in default.
/// 2018-05-22
/// Remove class singleton and renovate it to namespace for uniformation.
/// 2018-06-20 Change resource file style to json and also mechanism.
///

/// Header file
#include <Manager/resource_manager.h>

#include <fstream>
#include <optional>
#include <utility>

/// Third-party json library
#include <json.hpp>
/// Enhanced assertion.
#include <Phitos/Dbg/assert.h>

/// import logger in debug mode
#include <Headers/import_logger.h>
/// ::opgs16::helper:: string helper function.
#include <Helper/string_helper.h>
/// ::opgs16::helper::json json helper function.
#include <Helper/Json/json_helper.h>

/// ::opgs16::debug error messages.
#include <Manager/Internal/error_message.h>
/// ::opgs16::manager::_internal flags
#include <Manager/Internal/flag.h>
/// ::opgs16::builtin
#include <Manager/Internal/shader_builtin_keywords.h>

/// @todo remove this
#include <../manifest.h>

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Forward Declaration
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

using opgs16::resource::_internal::EResourceType;
using opgs16::resource::_internal::ESymbolType;
using phitos::enums::EFound;
using phitos::enums::ESucceed;

///
/// @brief
/// IsSceneEmpty already allocated resources and load resource with file path.
///
/// @param[in] file_path File path to load resource metadata.
/// @return success or failure flag.
///
ESucceed ReadResourceFile(const std::string& file_path);

///
/// @brief
///
/// @param[in] json
/// @param[in] file_path
///
/// @return
///
ESucceed VerifyResourceFile(const nlohmann::json& json,
                            const std::string& file_path);

///
/// @brief
///
/// @param[in] json
///
void InitializeFont(const nlohmann::json& json);

///
/// @brief
///
ESucceed VerifyFontInternalStructure(const nlohmann::json& json);

///
/// @brief
///
ESucceed VerifyFontInsertion(const nlohmann::json::const_iterator& it);

///
/// @brief
///
/// @param[in] json
///
void InitializeTexture(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] json
///
/// @return Success flag
///
ESucceed VerifyTextureInternalStructure(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] json
///
/// @return Sucess flag
///
ESucceed VerifyTextureInsertion(const nlohmann::json::const_iterator& json);

///
/// @brief
///
/// @param[in] json
///
/// @return
///
opgs16::resource::STexture2DAtlas
MakeTexture2D(const nlohmann::json::const_iterator& json);

///
/// @brief
///
/// @param[in] texture_instance
/// @param[in] atlas_path
///
/// @return Success flag
///
ESucceed
MakeTextureAtlasInformation(opgs16::resource::STexture2DAtlas& texture_instance,
                            const std::string& atlas_path);

///
/// @brief
///
/// @param[in] json
///
void InitializeMesh(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] container
///
///
void MakeTexelInformation(opgs16::resource::STexture2DAtlas& container);

///
/// @brief
///
/// @param[in] json
///
void InitializeSound(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] json
///
/// @return
///
opgs16::resource::SShader MakeShader(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] json
///
void InitializeShader(const nlohmann::json& json);

///
/// @brief
///
/// @param[in]
///
/// @return
///
ESucceed VerifySoundStructure(const nlohmann::json& json);

///
/// @brief
///
/// @param[in]
///
/// @return
///
ESucceed VerifySoundInsertion(const nlohmann::json::const_iterator& json);

///
/// @brief
///
/// @param[in]
///
/// @return
///
opgs16::resource::SSound MakeSound(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] json
///
/// @return
///
ESucceed VerifyShaderStructure(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] json
///
/// @return
///
ESucceed VerifyShaderInsertion(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] json
///
void InitializeAnimation(const nlohmann::json& json);

///
///
///
ESucceed VerifyAnimationStructure(const nlohmann::json& json);

///
///
///
ESucceed VerifyAnimationInsertion(const nlohmann::json& json);

///
/// @brief
///
opgs16::resource::SAnimationCell MakeAnimationCell(const nlohmann::json& json);

///
/// @brief
///
///
std::optional<opgs16::resource::SAnimation>
MakeAnimationContainer(const nlohmann::json& json);

///
/// @brief
///
///
/// @param[in] container
/// @param[in] name_key
///
/// @return
///
template <typename _Ty>
bool ExistKey(const std::map<std::string, _Ty>& container,
              const std::string& name_key);

//!
//! Datas
//!

///
/// This namespace is integrity check variable container for
/// checking runtime caveats of source code.
///
namespace {
using opgs16::debug::EInitiated;

const char* s_builtin   = "Setting/builtin.meta";
const char* s_resource  = "Setting/resource.meta";

constexpr const char* s_json_meta = "meta";

constexpr const char* s_json_font = "font";
constexpr const char* s_json_texture = "texture";
constexpr const char* s_json_mesh = "mesh";
constexpr const char* s_json_shader = "shader";
constexpr const char* s_json_sound = "sound";
constexpr const char* s_json_list = "list";
constexpr const char* s_json_animation = "animation";
constexpr const char* s_json_uniform = "uniform";

constexpr const char* s_json_path = "path";
constexpr const char* s_json_type = "type";
constexpr const char* s_json_atlas = "atlas";
constexpr const char* s_json_vs = "vs";
constexpr const char* s_json_fs = "fs";

constexpr const char* s_json_index = "index";
constexpr const char* s_json_texture_width = "texture_width";
constexpr const char* s_json_texture_height = "texture_height";
constexpr const char* s_json_offset_x = "offset_x";
constexpr const char* s_json_offset_y = "offset_y";
constexpr const char* s_json_width = "width";
constexpr const char* s_json_height = "height";

constexpr const char* s_json_st_effect = "effect";
constexpr const char* s_json_st_back = "back";

constexpr const char* s_json_texture_name = "texture_name";
constexpr const char* s_json_interval = "interval";

EInitiated m_initiated  = EInitiated::NotInitiated;
EInitiated m_shutdowned = EInitiated::NotInitiated;
} /// unnamed namespace

///
/// This namespace stores variables or
/// constexpr variables to be used by functions.
///
namespace {

using texture_map       = std::map<std::string, opgs16::resource::STexture2DAtlas>;
using sound_map         = std::map<std::string, opgs16::resource::SSound>;
using shader_map        = std::map<std::string, opgs16::resource::SShader>;
using font_map          = std::map<std::string, opgs16::resource::SFont>;
using animation_map     = std::map<std::string, opgs16::resource::SAnimation>;

/// Texture string container
texture_map m_textures;
/// Shader arguments container
shader_map  m_shaders;
/// Sounds path string container
sound_map   m_sounds;
/// Font path information container
static font_map    m_fonts;
/// Animation films information container
animation_map m_animations;

} /// unnamed namespace

//!
//! Implementation part
//!

namespace opgs16::manager::resource::__ {

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
      "Duplicated resource initiation is prohibited.");
  PHITOS_ASSERT(m_shutdowned == EInitiated::NotInitiated,
      "Reinitiation after shutdown is prohibited.");

  ReadResourceFile(
      helper::ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, s_builtin));
  ReadResourceFile(
      helper::ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, s_resource));

  m_initiated = EInitiated::Initiated;
}

void Shutdown() {
  m_animations.clear();
  m_textures.clear();
  m_shaders.clear();
  m_sounds.clear();
  m_fonts.clear();

  m_shutdowned = EInitiated::Initiated;
}

} /// ::opgs16::manager::resource::__ namespace

namespace opgs16::manager::resource {

const opgs16::resource::SShader* GetShader(const std::string& name_key) {
  if (!ExistKey(m_shaders, name_key)) {
    PUSH_LOG_ERROR_EXT("Did not find appropriate shader : [Tag : {0}]", name_key);
    return nullptr;
  }

  return &m_shaders[name_key];
}

const opgs16::resource::STexture2DAtlas* GetTexture2D(const std::string& name_key) {
  if (!ExistKey(m_textures, name_key)) {
    PUSH_LOG_ERROR_EXT("Did not find appropriate texture2D : [Tag : {0}]", name_key);
    return nullptr;
  }

  return &m_textures[name_key];
}

const opgs16::resource::SSound* GetSound(const std::string& name_key) {
  if (!ExistKey(m_sounds, name_key)) {
    PUSH_LOG_ERROR_EXT( "Did not find appropriate sound : [Tag : {0}]", name_key);
    return nullptr;
  }

  return &m_sounds[name_key];
}

const opgs16::resource::SFont* GetFont(const std::string& name_key) {
  if (ExistKey(m_fonts, name_key))
    return &m_fonts[name_key];

  PUSH_LOG_ERROR_EXT("Did not find appropriate proper font: [Tag : {0}]", name_key);
  return nullptr;
}

const opgs16::resource::SAnimation* GetAnimation(const std::string& name_key) {
  if (!ExistKey(m_animations, name_key)) {
    PUSH_LOG_ERROR_EXT("Did not find appropriate proper animation : [Tag : {0}]", name_key);
    throw nullptr;
  }

  return &m_animations[name_key];
}

} /// ::opgs16::manager::resource

//!
//! Free funciton definition part
//!

template <typename _Ty>
bool ExistKey(const std::map<std::string, _Ty>& container,
              const std::string& name_key) {
  return container.find(name_key) != container.end();
}

///
/// @brief Load json file safely.
/// If return value doesn't have a value, it specifies error in function.
///
/// @param[in] file_path Json file path to load.
///
/// @return Nullable nlohmann::json instance.
///
std::optional<nlohmann::json> LoadJsonFile(const std::string& file_path) {
  std::ifstream file_stream{ file_path, std::ios_base::in };
  file_stream.imbue(std::locale(""));
   if (!file_stream.good()) {
    PUSH_LOG_CRITICAL_EXT(
        "Failed to find project resource metafile. [{} : {}]", "Path", file_path);
    file_stream.close();
    return std::nullopt;
  }

  nlohmann::json atlas_json;
  file_stream >> atlas_json;
  file_stream.close();
  return atlas_json;
}

ESucceed ReadResourceFile(const std::string& file_path) {
  using phitos::enums::ESucceed;

  auto json_instance = LoadJsonFile(file_path);
  if (!json_instance.has_value()) {
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  const auto& atlas_json = json_instance.value();
  PUSH_LOG_INFO_EXT("Opened resource meta file. [Path : {}]", file_path);
  if (VerifyResourceFile(atlas_json, file_path) == ESucceed::Failed) {
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  InitializeFont(atlas_json[s_json_font]);
  InitializeTexture(atlas_json[s_json_texture]);
  InitializeMesh(atlas_json[s_json_mesh]);
  InitializeShader(atlas_json[s_json_shader]);
  InitializeSound(atlas_json[s_json_sound]);
#ifdef false
  InitializeAnimation(atlas_json[s_json_animation]);
#endif

  return ESucceed::Succeed;
}

ESucceed VerifyResourceFile(const nlohmann::json& json, const std::string& file_path) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  const auto CheckKeyAssert = [&](const char* key) {
    if (IsJsonKeyExist(json, key) == EFound::NotFound) {
      PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file. [{} : {}]", key, "Path", file_path);
      PHITOS_UNEXPECTED_BRANCH();
      return ESucceed::Failed;
    }
    return ESucceed::Succeed;
  };

  if (CheckKeyAssert(s_json_meta) == ESucceed::Failed) return ESucceed::Failed;
  if (CheckKeyAssert(s_json_font) == ESucceed::Failed) return ESucceed::Failed;
  if (CheckKeyAssert(s_json_texture) == ESucceed::Failed) return ESucceed::Failed;
  if (CheckKeyAssert(s_json_mesh) == ESucceed::Failed) return ESucceed::Failed;
  if (CheckKeyAssert(s_json_shader) == ESucceed::Failed) return ESucceed::Failed;
  if (CheckKeyAssert(s_json_sound) == ESucceed::Failed) return ESucceed::Failed;
  if (CheckKeyAssert(s_json_animation) == ESucceed::Failed) return ESucceed::Failed;

  return ESucceed::Succeed;
}

//!
//! Font
//!

void InitializeFont(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;

  if (VerifyFontInternalStructure(json) == ESucceed::Failed) {
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  // @todo Intentionally neglect font::meta header.
  // If meta information of font header is needed later, implement this.

  for (auto it = json[s_json_list].begin(); it != json[s_json_list].end(); ++it) {
    if (VerifyFontInsertion(it) == ESucceed::Failed) {
      continue;
    }

    using opgs16::resource::SFont;
    using opgs16::helper::ConcatDirectoryWithFile;
    auto [_, result] = m_fonts.try_emplace(it.key(), SFont(
        ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, it.value()[s_json_path].get<std::string>()),
        false
    ));

    if (result) {
      PUSH_LOG_DEBUG_EXT("Font {} is inserted as {} successfully.", it.key(), _->second.Path());
    }
    else {
      PUSH_LOG_ERROR_EXT("Failed to insert font {}.", it.key());
    }
  }
}

ESucceed VerifyFontInternalStructure(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_meta) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_font);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_list) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_list, s_json_font);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

ESucceed VerifyFontInsertion(const nlohmann::json::const_iterator& it) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  const auto& font_key = it.key();
  if (ExistKey(m_fonts, font_key)) {
    PUSH_LOG_ERROR_EXT("{0} key is duplicated. [{0} : {1}]", "Font", font_key);
    return ESucceed::Failed;
  }
  const auto& font_object = it.value();
  if (IsJsonKeyExist(font_object, s_json_path) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT("{0} is not found in {1} {2}. This {2} will be passed.",
        s_json_path, s_json_font, font_key);
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

//!
//! Texture
//!

void InitializeTexture(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;

  if (VerifyTextureInternalStructure(json) == ESucceed::Failed) {
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  // @todo Intentionally neglect font::meta header.
  // If meta information of font header is needed later, implement this.
  for (auto it = json[s_json_list].begin(); it != json[s_json_list].end(); ++it) {
    if (VerifyTextureInsertion(it) == ESucceed::Failed)
      continue;

    using opgs16::resource::STexture2DAtlas;
    using opgs16::helper::ConcatDirectoryWithFile;

    // Insert texture information.
    auto [_, result] = m_textures.try_emplace(it.key(), MakeTexture2D(it));

    if (result) {
      PUSH_LOG_DEBUG_EXT(
          "Font {} is inserted as {} successfully.",
          it.key(), _->second.path);
    }
    else {
      PUSH_LOG_ERROR_EXT("Failed to insert font {}.", it.key());
    }
  }
}

ESucceed VerifyTextureInternalStructure(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_meta) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_texture);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_list) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_texture);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

ESucceed VerifyTextureInsertion(const nlohmann::json::const_iterator& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  const auto& texture_key = json.key();
  if (ExistKey(m_textures, texture_key)) {
    PUSH_LOG_ERROR_EXT(
        "{0} key is duplicated. [{0} : {1}]",
        s_json_texture, texture_key);
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json.value(), s_json_path) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT(
        "{0} is not found in {1} {2}. This {2} will be passed.",
        s_json_path, s_json_texture, texture_key);
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

opgs16::resource::STexture2DAtlas
MakeTexture2D(const nlohmann::json::const_iterator& json) {
  using opgs16::helper::json::IsJsonKeyExist;
  using opgs16::helper::ConcatDirectoryWithFile;
  using opgs16::resource::STexture2DAtlas;

  static const auto MakeTexture2DSimple = [](STexture2DAtlas& atlas,
                                             const std::string& path) {
    atlas.path = path;
    atlas.fragment_number = 1;
    atlas.height = 0;
    atlas.width = 0;
  };

  STexture2DAtlas atlas;
  const std::string texture_path = ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, json.value()[s_json_path]);

  if (IsJsonKeyExist(json.value(), s_json_atlas) == EFound::NotFound) {
    MakeTexture2DSimple(atlas, texture_path);
  }
  else {
    const std::string atlas_path = ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, json.value()[s_json_atlas]);
    atlas.path = texture_path;

    if (MakeTextureAtlasInformation(atlas, atlas_path) == ESucceed::Failed) {
      atlas = STexture2DAtlas{};
      MakeTexture2DSimple(atlas, texture_path);
    }
  }

  return atlas;
}

ESucceed VerifyAtlasInformationStructure(const nlohmann::json& json) {
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_meta) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header. Atlas will be passed.", s_json_meta, s_json_texture);
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_list) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header. Atlas will be passed.", s_json_meta, s_json_texture);
    return ESucceed::Failed;
  }

  if (json[s_json_list].empty()) {
    PUSH_LOG_ERROR_EXT("Texture atlas fragment must have at leat one item.");
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

ESucceed VerifyAtlasItemStructure(const nlohmann::json& json) {
  using opgs16::helper::json::IsJsonKeyExist;
  static const auto CheckKeyExist = [](const nlohmann::json& json, const char* key) {
    if (IsJsonKeyExist(json, key) == EFound::NotFound) {
      PUSH_LOG_CRITICAL_EXT(
          "Header {} is not found in {} header. This atlas item will be passed.",
          key, s_json_list);
      return ESucceed::Failed;
    }
      return ESucceed::Succeed;
  };

  if (CheckKeyExist(json, s_json_offset_x) == ESucceed::Failed)
    return ESucceed::Failed;
  if (CheckKeyExist(json, s_json_offset_y) == ESucceed::Failed)
    return ESucceed::Failed;
  if (CheckKeyExist(json, s_json_width) == ESucceed::Failed)
    return ESucceed::Failed;
  if (CheckKeyExist(json, s_json_height) == ESucceed::Failed)
    return ESucceed::Failed;

  return ESucceed::Succeed;
}

ESucceed
MakeTextureAtlasInformation(opgs16::resource::STexture2DAtlas& texture_instance,
                            const std::string& atlas_path) {
  const auto json_atlas_optional = LoadJsonFile(atlas_path);
  if (!json_atlas_optional.has_value()) {
    return ESucceed::Failed;
  }
  const auto& atlas_json = json_atlas_optional.value();
  if (VerifyAtlasInformationStructure(atlas_json) == ESucceed::Failed) {
    return ESucceed::Failed;
  }

  texture_instance.has_atlas = true;
  texture_instance.width  = atlas_json[s_json_meta][s_json_texture_width];
  texture_instance.height = atlas_json[s_json_meta][s_json_texture_height];
  texture_instance.fragment_number = static_cast<uint32_t>(atlas_json[s_json_list].size());
  texture_instance.fragment.resize(texture_instance.fragment_number);
  texture_instance.texels.reserve(texture_instance.fragment_number);

  //
  const auto& list = atlas_json[s_json_list];
  for (auto it = list.begin(); it != list.end(); ++it) {
    const auto& key = it.key();
    const auto& value = it.value();
    if (VerifyAtlasItemStructure(value) == ESucceed::Failed)
      continue;

    opgs16::resource::STexture2DFragment fragment;
    fragment.name     = key;
    fragment.offset_x = value[s_json_offset_x].get<unsigned>();
    fragment.offset_y = value[s_json_offset_y].get<unsigned>();
    fragment.width    = value[s_json_width].get<unsigned>();
    fragment.height   = value[s_json_height].get<unsigned>();

    const auto index = value[s_json_index];
    texture_instance.fragment[index] = std::move(fragment);
  }

  MakeTexelInformation(texture_instance);
  return ESucceed::Succeed;
}

void MakeTexelInformation(opgs16::resource::STexture2DAtlas& container) {
  const auto texture_width = container.width;
  const auto texture_height = container.height;

  for (auto i = 0u, size = container.fragment_number; i < size; ++i) {
    opgs16::resource::STexture2DTexelInformation texel;
    const auto& info = container.fragment[i];

    const auto start_x = static_cast<float>(info.offset_x) / texture_width;
    const auto start_y = 1.f - (static_cast<float>(info.offset_y) / texture_height);
    const auto end_x = static_cast<float>(info.offset_x + info.width) / texture_width;
    const auto end_y = 1.f - (static_cast<float>(info.offset_y + info.height) / texture_height);
    texel.right_up[0] = end_x;
    texel.right_up[1] = start_y;
    texel.left_down[0] = start_x;
    texel.left_down[1] = end_y;

    container.texels.emplace_back(texel);
  }
}

//!
//! Mesh
//!

void InitializeMesh(const nlohmann::json& json) {
  PUSH_LOG_CRITICAL("Mesh importing is not supported yet.");
}

//!
//! Sound
//!

void InitializeSound(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;

  if (VerifyTextureInternalStructure(json) == ESucceed::Failed) {
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  // @todo Intentionally neglect font::meta header.
  // If meta information of sound header is needed later, implement this.

  for (auto it = json[s_json_list].begin(); it != json[s_json_list].end(); ++it) {
    if (VerifyTextureInsertion(it) == ESucceed::Failed)
      continue;

    using opgs16::resource::SSound;
    using opgs16::helper::ConcatDirectoryWithFile;

    // Insert texture information.

    auto [_, result] = m_sounds.try_emplace(it.key(), MakeSound(it.value()));

    if (result) {
      PUSH_LOG_DEBUG_EXT("{} {} is inserted as {} successfully.", "Sound", it.key(), _->second.Path());
    }
    else {
      PUSH_LOG_ERROR_EXT("Failed to insert sound {}.", it.key());
    }
  }
}

ESucceed VerifySoundStructure(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_meta) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_sound);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_list) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_sound);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

ESucceed VerifySoundInsertion(const nlohmann::json::const_iterator& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  const auto& key = json.key();
  if (ExistKey(m_sounds, key)) {
    PUSH_LOG_ERROR_EXT("{0} key is duplicated. [{0} : {1}]", s_json_sound, key);
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json.value(), s_json_path) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT(
        "{0} is not found in {1} {2}. This {2} will be passed.",
        s_json_path, s_json_sound, key);
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json.value(), s_json_type) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT(
        "{0} is not found in {1} {2}. This {2} will be passed.",
        s_json_type, s_json_sound, key);
    return ESucceed::Failed;
  }

  const auto type = json.value().get<std::string>();
  if (type != s_json_st_effect && type != s_json_st_back) {
    PUSH_LOG_ERROR_EXT("Sound item type is must be 'effect' or 'back'");
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

opgs16::resource::SSound MakeSound(const nlohmann::json& json) {
  using opgs16::helper::ConcatDirectoryWithFile;

  const auto local_path = json[s_json_path].get<std::string>();
  const auto type = json[s_json_type].get<std::string>();

  bool is_bgm = false;
  if (type == s_json_st_effect) {
    is_bgm = false;
  }
  else if (type == s_json_st_back) {
    is_bgm = true;
  }

  return opgs16::resource::SSound{
      ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, local_path),
      is_bgm
  };
}

//!
//! Shader
//!

void InitializeShader(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;

  if (VerifyShaderStructure(json) == ESucceed::Failed) {
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  // @todo Intentionally neglect font::meta header.
  // If meta information of sound header is needed later, implement this.

  for (auto it = json[s_json_list].begin(); it != json[s_json_list].end(); ++it) {
    if (VerifyShaderInsertion(it.value()) == ESucceed::Failed)
      continue;

    using opgs16::resource::SSound;
    using opgs16::helper::ConcatDirectoryWithFile;

    // Insert texture information.

    auto [_, result] = m_shaders.try_emplace(it.key(), MakeShader(it.value()));

    if (result) {
      PUSH_LOG_DEBUG_EXT("{} {} is inserted as successfully.", "Shader", it.key());
    }
    else {
      PUSH_LOG_ERROR_EXT("Failed to insert sound {}.", it.key());
    }
  }
}

ESucceed VerifyShaderStructure(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_meta) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_shader);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_list) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_shader);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

ESucceed VerifyShaderInsertion(const nlohmann::json& json) {
  static const auto CheckKeyExist = [](const nlohmann::json& json, const char* key) {
    using opgs16::helper::json::IsJsonKeyExist;
    if (IsJsonKeyExist(json, key) == EFound::NotFound) {
      PUSH_LOG_CRITICAL_EXT(
          "Header {} is not found in {} header. This {} item will be passed.",
          key, s_json_list, "Shader");
      return ESucceed::Failed;
    }
      return ESucceed::Succeed;
  };

  if (CheckKeyExist(json, s_json_vs) == ESucceed::Failed) {
    PUSH_LOG_CRITICAL_EXT("Shader must have vertex shader.");
    return ESucceed::Failed;
  }
  if (CheckKeyExist(json, s_json_fs) == ESucceed::Failed) {
    PUSH_LOG_CRITICAL_EXT("Shader must have fragment shader.");
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

static opgs16::resource::SShader::TVariableList
GetCustomUniformVariableList(const std::string& file_path) {
  using opgs16::helper::json::IsJsonKeyExist;

  auto json_instance = LoadJsonFile(file_path);
  if (!json_instance.has_value()) {
    PHITOS_UNEXPECTED_BRANCH();
    return {};
  }

  const auto& atlas_json = json_instance.value();
  PUSH_LOG_INFO_EXT("Opened resource meta file. [Path : {}]", file_path);

  if (IsJsonKeyExist(atlas_json, s_json_uniform) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT(
        "Could not find 'uniform' header from shader smeta file. [Path : {}]",
        file_path);
    return {};
  }

  using TVariableList = opgs16::resource::SShader::TVariableList;
  TVariableList var_list;

  auto& uniform = atlas_json[s_json_uniform];
  for (auto it = uniform.begin(); it != uniform.end(); ++it) {
    const std::string& key = it.key();

    if (var_list.find(key) != var_list.end()) {
      PUSH_LOG_ERROR_EXT("Duplicated variable name. [Name : {}]", key);
      continue;
    }

    using opgs16::builtin::s_shader_builtin;
    if (std::any_of(s_shader_builtin.begin(), s_shader_builtin.end(),
        [&key](const std::string_view& builtin_variable) {
          return key == builtin_variable;
        }
    )) {
      PUSH_LOG_ERROR_EXT(
          "Builtin variable name redifinition is prohibited. [Name : {}]",
          key);
      continue;
    }

    using opgs16::resource::SShader;
    if (auto result = std::find(
            SShader::s_variable_str,
            SShader::s_variable_str + 11,
            it.value().get<std::string>());
        result == SShader::s_variable_str + 11) {
      PUSH_LOG_ERROR_EXT(
          "Could not find appropriate shader variable type. [Name : {}]",
          key);
    }
    else {
      auto index = std::distance(SShader::s_variable_str, result);
      using EVariableType = opgs16::resource::SShader::EVariableType;
      var_list.try_emplace(key, static_cast<EVariableType>(index));
    }
  }

  return var_list;
}

opgs16::resource::SShader MakeShader(const nlohmann::json& json) {
  using opgs16::helper::ConcatDirectoryWithFile;
  using opgs16::helper::json::IsJsonKeyExist;
  using opgs16::resource::EShaderType;

  opgs16::resource::SShader::TShaderList shader_list;
  shader_list.emplace_back(EShaderType::VS,
      ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, json[s_json_vs]));
  shader_list.emplace_back(EShaderType::FS,
      ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, json[s_json_fs]));

  if (IsJsonKeyExist(json, s_json_meta) == EFound::Found) {
    const auto path =
        ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH, json[s_json_meta]);

    return opgs16::resource::SShader{
        shader_list,
        GetCustomUniformVariableList(path)
    };
  }

  return opgs16::resource::SShader{ shader_list };
}

//!
//! Animation
//!

void InitializeAnimation(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;

  if (VerifyAnimationStructure(json) == ESucceed::Failed) {
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  // @todo Intentionally neglect font::meta header.
  // If meta information of font header is needed later, implement this.
  for (auto it = json[s_json_list].begin(); it != json[s_json_list].end(); ++it) {
    // @todo Integrity check
    auto container = MakeAnimationContainer(it.value());
    if (container.has_value()) {
      auto [_, result] = m_animations.try_emplace(it.key(), container.value());

      if (result) {
        PUSH_LOG_DEBUG_EXT("Animation {} is inserted successfully.", it.key());
      }
      else {
        PUSH_LOG_ERROR_EXT("Failed to insert animation {}.", it.key());
      }
    }
  }
}

ESucceed VerifyAnimationStructure(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_meta) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_animation);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_list) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in {} header.", s_json_meta, s_json_animation);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  const auto& list = json[s_json_list];
  for (auto it = list.begin(); it != list.end(); ++it) {
    if (!it.value().is_object()) {
      PUSH_LOG_CRITICAL_EXT("Animation cell item in list must be a object type.");
      return ESucceed::Failed;
    }
  }

  return ESucceed::Succeed;
}

ESucceed VerifyAnimationInsertion(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_texture_name) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT(
      "Header {} is not found in cell object. This {} item will be passed.",
      s_json_texture_name, "animation cell");
    return ESucceed::Failed;
  }

  if (!json[s_json_texture_name].is_string()) {
    PUSH_LOG_CRITICAL_EXT("Header {} value must be a string.", s_json_texture_name);
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_index) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT(
      "Header {} is not found in cell object. This {} item will be passed.",
      s_json_index, "animation cell");
    return ESucceed::Failed;
  }

  if (!json[s_json_index].is_number_unsigned()) {
    PUSH_LOG_CRITICAL_EXT("Header {} value must be a unsigned integer.", s_json_index);
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_interval) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT(
      "Header {} is not found in cell object. This {} item will be passed.",
      s_json_interval, "animation cell");
    return ESucceed::Failed;
  }

  if (!json[s_json_interval].is_number_unsigned()) {
    PUSH_LOG_CRITICAL_EXT("Header {} value must be a unsigned integer.", s_json_interval);
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

std::optional<opgs16::resource::SAnimation>
MakeAnimationContainer(const nlohmann::json& json) {
  auto json_atlas = LoadJsonFile(json[s_json_path]);
  if (!json_atlas.has_value()) {
    return std::nullopt;
  }

  opgs16::resource::SAnimation animation;
  for (auto it = json[s_json_list].begin(); it != json[s_json_list].end(); ++it) {
    if (VerifyAnimationInsertion(it.value()) == ESucceed::Failed)
      continue;

    animation.cells.emplace_back(MakeAnimationCell(it.value()));
  }

  return animation;
}

opgs16::resource::SAnimationCell MakeAnimationCell(const nlohmann::json& json) {
  return { json[s_json_texture_name], json[s_json_index], json[s_json_interval] };
}
