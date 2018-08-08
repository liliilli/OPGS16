#ifndef OPGS16_SYSTEM_FRAME_TEXTURE_H
#define OPGS16_SYSTEM_FRAME_TEXTURE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/texture2d_sprite.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-23 m_texture::Texture2D Refactoring
/// 2018-02-28 Add Width, Height inline functions.
/// 2018-03-10 Refactoring.
/// 2018-04-06 IndexSize to unsigned value because fragmentation mechanism has been changed.
/// 2018-04-06 Change constructor parameter type STexture2D to STexture2DAtlas.
/// 2018-08-08 Update file
///

#include <optional>
#include <vector>

#include <Element/Internal/texture_base.h>
#include <Manager/ResourceType/texture.h>
#include <Helper/Type/vector2.h>

///
/// @namespace texture
/// @brief Namespace for m_texture resource class.
///
namespace opgs16::texture {

///
/// @class CTexture2DSprite
/// @brief The class for binding Texture id and can set m_texture's parameters.
///
class CTexture2DSprite : public element::_internal::CTextureBase {
public:
  enum class ETexelType {
    LeftDown,
    RightUp
  };

  CTexture2DSprite() = default;
  CTexture2DSprite(const CTexture2DSprite&) = default;
  CTexture2DSprite(CTexture2DSprite&&) = default;
  CTexture2DSprite& operator=(const CTexture2DSprite&) = default;
  CTexture2DSprite& operator=(CTexture2DSprite&&) = default;
  ~CTexture2DSprite();

  ///
	/// @brief Constructor get path and m_texture mode as arguments, build and set them.
  /// @param[in] container Container which stores texture atlas information.
	///
  bool Initialize(const resource::STexture2DAtlas& container);

  ///
  /// @brief Get Texel information from m_texels with given index number.
  /// If given index number is out of bound, return nullptr.
  /// And is_texture_atlas is false so this texture is not concerned with atlas, return nullptr;
  ///
  std::optional<DVector2> GetTexelPtr(ETexelType texel_type, int32_t index) const noexcept;

  ///
  /// @brief Return whether this texture handles atlas information or not.
  ///
  bool DoesHasAtlas() const noexcept {
    return is_texture_atlas;
	}

private:
  bool is_texture_atlas = false;
  const std::vector<resource::STexture2DTexelInformation>* m_texels = nullptr;
};

} /// ::opgs16::texture namespace

#endif /// OPGS16_SYSTEM_FRAME_TEXTURE_H