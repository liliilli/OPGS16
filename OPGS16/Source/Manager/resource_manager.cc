
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
///

#include <Manager\resource_manager.h>   /// Header file

#include <fstream>
#include <sstream>
#include <stdexcept>	
#include <utility>

/// Third-party json library
#include "..\..\..\__ThirdParty\nlohmann\json.hpp"  

/// import logger in debug mode
#include <Headers\import_logger.h> 
/// Expanded assertion
#include <Helper\assert.h>
/// ::opgs16::debug error messages.
#include <Manager\Internal\error_message.h>
/// ::opgs16::manager::_internal flags
#include <Manager\Internal\flag.h>
/// ::opgs16::resource::_internal namespace
#include <Manager\Internal\resource_internal.h>

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Forward Declaration
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

using opgs16::resource::_internal::EResourceType;
using opgs16::resource::_internal::ESymbolType;

///
/// @brief
/// Read resource along with resource_type
///
/// @param[in] token_line
/// @param[in] stream
/// @param[in] global_path
/// @param[in] type
///
void ReadResource(const std::string& token_line, 
                  std::ifstream& stream,
                  const std::string& global_path, 
                  EResourceType type);

///
/// @brief 
/// Read symbol and return with token string.
///
/// @param[in] line_token
///
/// @return
///
std::pair<ESymbolType, std::string> ReadSymbol(const std::string& line_token);


///
/// @brief
/// Get resource type from token.
///
/// @param[in] resource_token
///
/// @return EResourceType
///
EResourceType GetResourceType(const std::string_view& resource_token);

///
/// @brief
///
/// @param[in] name_key
/// @param[in] list
///
void PushShader(const std::string& name_key, 
                const opgs16::resource::SShader& list);

///
/// @brief
///
/// @param[in] name_key
/// @param[in] container 
///
void PushTexture2D(const std::string& name_key, 
                   const opgs16::resource::STexture2DAtlas& container);

///
/// @brief
///
/// @param[in] name_key
/// @param[in] container 
///
void PushSound(const std::string& name_key, 
               const opgs16::resource::SSound& container);

///
/// @brief
///
/// @param[in] name_key
/// @param[in] container 
///
void PushFont(const std::string& name_key, 
              const opgs16::resource::SFont& container);

///
/// @brief
///
/// @param[in] name_key
/// @param[in] container 
///
void PushAnimation(const std::string& name_key, 
                   const opgs16::resource::SAnimation& container);

///
/// @brief 
/// Make Texture2D without any atlas information.
/// Atlas information will be filled with default information.
///
/// @param[in] line_stream
/// @param[in] global_path
///
/// @return 
///
opgs16::resource::STexture2DAtlas MakeTexture2DContainerDefault(
    std::stringstream& line_stream, 
    const std::string& global_path);

///
/// @brief
/// Create atlas information from json stream.
///
/// @param[in] atlas_json
/// @param[in] list_name
/// @param[in] resource
///
/// @throw std::runtime_error
///
template <typename T>
std::vector<T> CreateAtlasInformationList(
    nlohmann::json& atlas_json,
    const std::string& list_name,
    opgs16::resource::STexture2DAtlas* resource);

///
/// @brief
/// 
/// @param[in] container
///
///
void MakeTexelInformation(opgs16::resource::STexture2DAtlas& container);

///
/// @brief 
/// Make Texture2D atlas information.
/// if something is wrong, generate runtime_error.
///
/// @param[in] file
/// @param[in] path
///
/// @return
///
opgs16::resource::STexture2DAtlas MakeTextureAtlasInformation(
    std::ifstream& file,
    const std::string& path);

///
/// @brief 
/// Make Texture2D with specific atlas information
///
/// @param[in] line_stream
/// @param[in] global_path
///
/// @return
///
/// @throw std::runtime_error
///
opgs16::resource::STexture2DAtlas MakeTexture2DAtlasContainer(
    std::stringstream& line_stream,
    const std::string& global_path);

///
/// @brief
/// Make Sound as background music
///
/// @param[in] line_stream
/// @param[in] global_path
/// @param[in] is_bgm
///
/// @return
///
opgs16::resource::SSound MakeSoundContainer(
    std::stringstream& line_stream,
    const std::string& global_path,
    bool is_bgm);

///
/// @brief 
/// Make shader container
///
/// @param[in] stream
/// @param[in] global_path
/// @param[in] scope_type
///
/// @return
///
opgs16::resource::SShader MakeShaderContainer(
    std::ifstream& stream,
    const std::string& global_path);

///
/// @brief 
/// Make font container
///
/// @param[in] line_stream
/// @param[in] global_path
///
/// @return
///
opgs16::resource::SFont MakeFontContainer(
    std::stringstream& line_stream,
    const std::string& global_path);

///
/// @brief 
/// Make animation film container.
///
/// @param[in] line_stream
/// @param[in] global_path
///
/// @return
///
opgs16::resource::SAnimation MakeAnimationContainer(
    std::stringstream& line_stream,
    const std::string& global_path);
  
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

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// This namespace is integrity check variable container for 
/// checking runtime caveats of source code.
///
namespace {
using opgs16::debug::EInitiated;

EInitiated m_initiated = EInitiated::NotInitiated;
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
font_map    m_fonts;
/// Animation films information container
animation_map m_animations;

} /// unnamed namespace

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Definition part
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

namespace opgs16::manager::resource {

bool ReadResourceFile(const std::string& file_path) {
  std::ifstream file_stream{ file_path, std::ios_base::in };
  file_stream.imbue(std::locale(""));

  if (!file_stream.good()) {
    PUSH_LOG_ERRO((std::wstring{ L"Cannot open the file : " }).c_str());
    return false;
  }

  PUSH_LOG_WARN((std::wstring{ L"Opened resource meta file : " }).c_str());

  std::string global_path, file_line;
  while (std::getline(file_stream, file_line)) {
    if (file_line.empty()) continue;

    switch (auto[symbol, token] = ReadSymbol(file_line); symbol) {
    default: break;
    case opgs16::resource::_internal::ESymbolType::GLOBAL_PATH:
      global_path = token;
      PUSH_LOG_WARN(std::wstring(global_path.begin(), global_path.end()).c_str());
      break;
    case opgs16::resource::_internal::ESymbolType::RESOURCE:
      ReadResource(file_line, file_stream, global_path, GetResourceType(token));
      break;
    }
  }

  return file_stream.eof();
}

const opgs16::resource::SShader& GetShader(const std::string& name_key) {
  if (!ExistKey(m_shaders, name_key)) {
#if defined(_DEBUG)
    std::wstring log{ L"Does not found proper shader, " };
    log.append(std::cbegin(name_key), std::cend(name_key));
    PushLog(LOG_TYPE_ERRO, log.c_str());
    PushLog(LOG_TYPE_ERRO, L"Return dummy shader...");
#endif /*! Log error message in Debug mode */
        /*! Temporary */
    throw std::runtime_error("GetShader error");
  }

  return m_shaders[name_key];
}

const opgs16::resource::STexture2DAtlas& GetTexture2D(const std::string& name_key) {
  if (!ExistKey(m_textures, name_key)) {
#if defined(_DEBUG)
    std::wstring log{ L"Does not found proper texture2D, " };
    log.append(std::cbegin(name_key), std::cend(name_key));
    PushLog(LOG_TYPE_ERRO, log.c_str());
#endif /*! Log error message in Debug mode */
        /*! Temporary */
    throw std::runtime_error("GetTexture2D error");
  }

  return m_textures[name_key];
}

const opgs16::resource::SSound& GetSound(const std::string& name_key) {
  if (!ExistKey(m_sounds, name_key)) {
#if defined(_DEBUG)
    std::wstring log{ L"Does not found proper sound, " };
    log.append(std::cbegin(name_key), std::cend(name_key));
    PushLog(LOG_TYPE_ERRO, log.c_str());
#endif /*! Log error message in Debug mode */
        /* Temporary */
    throw std::runtime_error("GetSound error");
  }

  return m_sounds[name_key];
}

std::pair<bool, const opgs16::resource::SFont*> GetFont(const std::string & name_key) {
  if (ExistKey(m_fonts, name_key))
    return { true, &m_fonts[name_key] };
  else
    return { false, nullptr };
}

const opgs16::resource::SAnimation* GetAnimation(const std::string& name_key) {
  if (!ExistKey(m_animations, name_key)) {
#if defined(_DEBUG)
    std::wstring log{ L"Does not found proper sound, " };
    log.append(std::cbegin(name_key), std::cend(name_key));
    PUSH_LOG_ERRO(log.c_str());
#endif /*! Log error message in Debug mode */
        /* Temporary */
    throw std::runtime_error("GetSound error");
  }

  return &m_animations[name_key];
}

} /// ::opgs16::manager::resource

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Free function definition part
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

template <typename _Ty>
bool ExistKey(const std::map<std::string, _Ty>& container, 
              const std::string& name_key) {
  return container.find(name_key) != container.end();
}

std::pair<ESymbolType, std::string> ReadSymbol(const std::string& line_token) {
    ESymbolType symbol{ ESymbolType::NOTHING };

    std::stringstream line_stream{ line_token };
    std::string token; line_stream >> token;

    switch (*token.cbegin()) {
    default: symbol = ESymbolType::RESOURCE; break;
    case '$':   /*! This statements is defualt resource path. */
        symbol = ESymbolType::GLOBAL_PATH;
        line_stream >> token;
        break;
    case '#':   /*! This statements is comment line. */
        symbol = ESymbolType::COMMENT;
        break;
    }

    return { symbol, token };
}

void ReadResource(const std::string& token_line, std::ifstream& stream,
                  const std::string& global_path, EResourceType type) {
  std::stringstream line_stream{ token_line };
  /*! Just drop first token */
  {
    std::string __;
    line_stream >> __;
  }

  switch (type) {
  default: break;
  case EResourceType::TEXTURE_2D: {    /*! Texture 2D information generating sequence */
    std::string tag;    line_stream >> tag;
    PushTexture2D(tag, MakeTexture2DContainerDefault(line_stream, global_path));
  }   break;
  case EResourceType::TEXTURE_2D_ATLAS: {
    std::string tag;    line_stream >> tag;
    PushTexture2D(tag, MakeTexture2DAtlasContainer(line_stream, global_path));
  }   break;
  case EResourceType::SHADER: {
    std::string tag;    line_stream >> tag;
    PushShader(tag, MakeShaderContainer(stream, global_path));
  }   break;
  case EResourceType::SOUND_EFFECT_BGM: {
    std::string tag;    line_stream >> tag;
    PushSound(tag, MakeSoundContainer(line_stream, global_path, true));
  }   break;
  case EResourceType::SOUND_EFFECT_EFFECT: {
    std::string tag;    line_stream >> tag;
    PushSound(tag, MakeSoundContainer(line_stream, global_path, false));
  }   break;
  case EResourceType::FONT: {
    std::string tag;    line_stream >> tag;
    PushFont(tag, MakeFontContainer(line_stream, global_path));
  }   break;
  case EResourceType::ANIMATION: {
    std::string tag;    line_stream >> tag;
    PushAnimation(tag, MakeAnimationContainer(line_stream, global_path));
  }   break;
  }
}

void PushShader(const std::string& name_key, 
                const opgs16::resource::SShader& list) {
  if (ExistKey(m_shaders, name_key)) {
#if defined(_DEBUG)
    PushLog(LOG_TYPE_ERRO, L"Shader key duplicated");
#endif
    throw std::runtime_error("Shader Key duplicated :: " + name_key);
  }

  m_shaders.emplace(name_key, list);
}

void PushTexture2D(const std::string& name_key, 
                   const opgs16::resource::STexture2DAtlas& container) {
  if (ExistKey(m_textures, name_key))
    throw std::runtime_error("Texture Key duplicated :: " + name_key);

  m_textures.emplace(name_key, container);
}

void PushSound(const std::string& name_key, 
               const opgs16::resource::SSound& container) {
  if (ExistKey(m_sounds, name_key))
    throw std::runtime_error("Sound Key duplicated :: " + name_key);

  m_sounds.emplace(name_key, container);
}

void PushFont(const std::string& name_key, 
              const opgs16::resource::SFont& container) {
  if (ExistKey(m_fonts, name_key))
    throw std::runtime_error("Font Key duplicated :: " + name_key);

  m_fonts.emplace(name_key, container);
}

void PushAnimation(const std::string& name_key, 
                   const opgs16::resource::SAnimation& container) {
  if (ExistKey(m_animations, name_key))
    throw std::runtime_error("Animation duplicated :: " + name_key);

  m_animations.emplace(name_key, container);
}

EResourceType GetResourceType(const std::string_view& resource_token) {
    using namespace opgs16::resource::_internal;

    if (resource_token == TEX2D)
      return EResourceType::TEXTURE_2D;
    if (resource_token == TEX2DATLAS)
      return EResourceType::TEXTURE_2D_ATLAS;
    if (resource_token == SHADE)
      return EResourceType::SHADER;
    if (resource_token == SEBGM)
      return EResourceType::SOUND_EFFECT_BGM;
    if (resource_token == SEEFF)
      return EResourceType::SOUND_EFFECT_EFFECT;
    if (resource_token == FONT)
      return EResourceType::FONT;
    if (resource_token == ANIMATION)
      return EResourceType::ANIMATION;

    return EResourceType::NOTHING;
}

opgs16::resource::STexture2DAtlas MakeTexture2DContainerDefault(
    std::stringstream& line_stream, 
    const std::string& global_path) {
  std::string local_path; 
  line_stream >> local_path;

  // Print log in _DEBUG mode
#if defined(_DEBUG)
  std::wstring log{ L"[Texture2D][" };
  log.append(local_path.cbegin(), local_path.cend());     log.append(L"]");
  PushLog(LOG_TYPE_INFO, log.c_str());
#endif 

  opgs16::resource::STexture2DAtlas atlas_resource;
  atlas_resource.path = global_path + local_path;
  atlas_resource.fragment_number = 1;
  atlas_resource.height = 0;
  atlas_resource.width = 0;

  return atlas_resource;
};

template <typename T>
std::vector<T> CreateAtlasInformationList(
    nlohmann::json& atlas_json, 
    const std::string& list_name,
    opgs16::resource::STexture2DAtlas* resource) {

  if (!atlas_json.at(list_name).is_array()) {
    PushLog(LOG_TYPE_ERRO, L"Invalid type specifier, offset_x must be array.");
    throw std::runtime_error("Invalid type specifier, offset_x must be array.");
  }

  const auto list = atlas_json[list_name.c_str()].get<std::vector<T>>();
  if (list.size() != resource->fragment_number) {
    PushLog(LOG_TYPE_ERRO, L"offset_x list size must be same as fragment_number value.");
    throw std::runtime_error("List size is not same as fragment_number.");
  }

  return list;
}

void MakeTexelInformation(opgs16::resource::STexture2DAtlas& container) {
  const auto texture_width = container.width;
  const auto texture_height = container.height;

  for (auto i = 0u, size = container.fragment_number; i < size; ++i) {
    auto& texel = container.texels[i];
    const auto& info = container.fragment[i];

    const auto start_x = static_cast<float>(info.offset_x) / texture_width;
    const auto start_y = 1.f - (static_cast<float>(info.offset_y) / texture_height);
    const auto end_x = static_cast<float>(info.offset_x + info.width) / texture_width;
    const auto end_y = 1.f - (static_cast<float>(info.offset_y + info.height) / texture_height);
    texel.right_up[0] = end_x;
    texel.right_up[1] = start_y;
    texel.left_down[0] = start_x;
    texel.left_down[1] = end_y;
  }
}

opgs16::resource::STexture2DAtlas MakeTextureAtlasInformation(
    std::ifstream& file, 
    const std::string& path) {
  nlohmann::json atlas_json;
  file >> atlas_json;

  auto atlas_resource = opgs16::resource::STexture2DAtlas{};
  atlas_resource.has_atlas = true;
  atlas_resource.width = atlas_json.at("texture_width").get<unsigned>();
  atlas_resource.height = atlas_json.at("texture_height").get<unsigned>();
  atlas_resource.fragment_number = atlas_json.at("item_number").get<unsigned>();
  atlas_resource.fragment.resize(atlas_resource.fragment_number);
  atlas_resource.texels.resize(atlas_resource.fragment_number);
  atlas_resource.path = path;

  auto offset_x_list = CreateAtlasInformationList<unsigned>(atlas_json, "offset_x", &atlas_resource);
  auto offset_y_list = CreateAtlasInformationList<unsigned>(atlas_json, "offset_y", &atlas_resource);
  auto width_list = CreateAtlasInformationList<unsigned>(atlas_json, "fragment_width", &atlas_resource);
  auto height_list = CreateAtlasInformationList<unsigned>(atlas_json, "fragment_height", &atlas_resource);
  auto name_list = CreateAtlasInformationList<std::string>(atlas_json, "name", &atlas_resource);
  for (auto i = 0u; i < atlas_resource.fragment_number; ++i) {
    atlas_resource.fragment[i].offset_x = offset_x_list[i];
    atlas_resource.fragment[i].offset_y = offset_y_list[i];
    atlas_resource.fragment[i].width = width_list[i];
    atlas_resource.fragment[i].height = height_list[i];
    atlas_resource.fragment[i].name = name_list[i];
  }
  MakeTexelInformation(atlas_resource);
  return atlas_resource;
}

opgs16::resource::STexture2DAtlas MakeTexture2DAtlasContainer(
    std::stringstream& line_stream, 
    const std::string& global_path) {

  std::string local_texture_path; line_stream >> local_texture_path;
  std::string local_atlas_path; line_stream >> local_atlas_path;
#if defined(_DEBUG)
  {
    std::wstring log; log.append(L"[Texture2DAtlas][");
    log.append(local_texture_path.cbegin(), local_texture_path.cend()); log.append(L"][");
    log.append(local_atlas_path.cbegin(), local_atlas_path.cend()); log.append(L"]");
    PUSH_LOG_INFO(log.c_str());
  }
#endif /*! Print local pathes */

  std::string atlas_path = global_path + local_atlas_path;
  std::ifstream file{ atlas_path.c_str(), std::ios_base::in };
  if (file.bad()) {
    std::wstring log; log.resize(150); log.append(L"Failed to read ");
    log.append(atlas_path.cbegin(), atlas_path.cend());
    PushLog(LOG_TYPE_ERRO, log.c_str());
    throw std::runtime_error("Failed to read resource file.");
  }

  return MakeTextureAtlasInformation(file, global_path + local_texture_path);
}

opgs16::resource::SSound MakeSoundContainer(
    std::stringstream& line_stream, 
    const std::string& global_path,
    bool is_bgm) {

    std::string local_path; line_stream >> local_path;
#if defined(_DEBUG)
    {
        std::wstring log{ L"[Sound][" };
        log.append(local_path.cbegin(), local_path.cend());     log.append(L"]");
        PushLog(LOG_TYPE_INFO, log.c_str());
    }
#endif
    return opgs16::resource::SSound{ global_path + local_path, is_bgm };
};

opgs16::resource::SShader MakeShaderContainer(
    std::ifstream& stream, 
    const std::string& global_path) {

  opgs16::resource::SShader::shader_list shader_list;

  while (true) {
    std::string token_line;
    std::getline(stream, token_line);
    if (token_line.empty() || (token_line.at(0) != '-')) break;
    else {  /*! token_line is not empty and their is something. */
      std::stringstream line_stream{ token_line };
      std::string not_use, shader_token, local_path;
      line_stream >> not_use >> shader_token >> local_path;

      shader_list.emplace_back(opgs16::resource::GetShaderType(shader_token), global_path + local_path);
    }
  }
#if defined(_DEBUG)
  {
    PushLog(LOG_TYPE_INFO, L"[SHADER]");
    for (const auto& shader : shader_list) {
      std::wstring log{ L"[SHADER_ELEMENT][" };
      log.append(shader.second.cbegin(), shader.second.cend()); log.append(L"]");
      PushLog(LOG_TYPE_INFO, log.c_str());
    }
  }
#endif
  return opgs16::resource::SShader{ shader_list };
};

opgs16::resource::SFont MakeFontContainer(
    std::stringstream& line_stream, 
    const std::string& global_path) {

  std::string local_path; 
  line_stream >> local_path;

  bool is_default;
  line_stream >> is_default;

#if defined(_DEBUG)
  {
    std::wstring log{ L"[FontGlyph][" };
    log.append(local_path.cbegin(), local_path.cend());     log.append(L"][");
    log.append(is_default ? L"DEFAULT" : L"NOT_DEFAULT");   log.append(L"]");
    PushLog(LOG_TYPE_INFO, log.c_str());
  }
#endif
  return opgs16::resource::SFont{ global_path + local_path, is_default };
};

opgs16::resource::SAnimation MakeAnimationContainer(
    std::stringstream& line_stream, 
    const std::string& global_path) {

  std::string path; line_stream >> path; path = global_path + path;
  std::ifstream animation_file{ path, std::ios_base::in };
  if (!animation_file.good()) {
#if defined(_DEBUG)
    {
      std::wstring log{ L"Failed to load animation file. " };
      log.append(std::cbegin(path), std::cend(path));
      PUSH_LOG_ERRO(log.c_str());
    }
#endif
    throw std::runtime_error{ "Failed to read animation file." };
  }

  opgs16::resource::SAnimation container;
  unsigned total_cell{ 0 }, total_time{ 0 };
  std::string line;
  while (std::getline(animation_file, line)) {
    if (line.empty() || line[0] == '#') continue; /*! Empty line || Continue */
    std::stringstream animation_stream{ line };
    std::string texture2d_name; unsigned index, time_milli;
    animation_stream >> texture2d_name >> index >> time_milli;
    container.cells.emplace_back(opgs16::resource::SAnimationCell{ texture2d_name, index, time_milli });

    ++total_cell;
    total_time += time_milli;
  }
#if defined(_DEBUG)
  {
    std::wstring log{ L"[Animation][" };
    log.append(std::cbegin(path), std::cend(path)); log += L"][Total:";
    log += std::to_wstring(total_cell); log += L"][Time:";
    log += std::to_wstring(total_time); log += L"]";
    PUSH_LOG_INFO(log.c_str());
  }
#endif
  return container;
}
