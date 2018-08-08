#ifndef OPGS16_MANAGER_RESOURCETYPE_TEXTURE_H
#define OPGS16_MANAGER_RESOURCETYPE_TEXTURE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/ResourceType/texture.h
///
/// @author Jongmin Yun
///

namespace opgs16::resource {

struct STexture2DFragment {
  std::string name;
  unsigned offset_x, offset_y;
  unsigned width, height;
};

struct alignas(16) STexture2DTexelInformation {
  float left_down[2]  = { 0.f, 0.f };
  float right_up[2]   = { 1.f, 1.f };
};

///
/// @struct STexture2DAtlas
/// @brief Information structure of Texture2D with atlas information (if not, fill it with default)
///
struct STexture2DAtlas final {
  bool has_atlas = false;

  int32_t width = 0;
  int32_t height = 0;
  int32_t fragment_number = 0;
  std::vector<STexture2DFragment> fragment{};
  std::vector<STexture2DTexelInformation> texels{};
  std::string path{};
  std::string name{};
};

} /// ::opgs16::resource namespace

#endif /// OPGS16_MANAGER_RESOURCETYPE_TEXTURE_H