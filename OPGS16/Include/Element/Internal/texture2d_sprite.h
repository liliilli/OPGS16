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
#include <GL/glew.h>

#include <Element/Internal/texture_internal.h>
#include <Manager/resource_type.h>
#include <Helper/Type/vector2.h>
#include <opgs16fwd.h>

///
/// @namespace texture
/// @brief Namespace for m_texture resource class.
///
namespace opgs16::texture {

///
/// @class CTexture2DSprite
/// @brief The class for binding Texture id and can set m_texture's parameters.
///
class CTexture2DSprite {
  using TextureParameter = element::texture::_internal::DTextureParameter;

public:
  enum class ETexelType {
    LeftDown,
    RightUp
  };

  CTexture2DSprite(const CTexture2DSprite&) = default;
  CTexture2DSprite(CTexture2DSprite&&) = default;
  CTexture2DSprite& operator=(const CTexture2DSprite&) = default;
  CTexture2DSprite& operator=(CTexture2DSprite&&) = default;

  ~CTexture2DSprite();

  ///
	/// @brief Constructor get path and m_texture mode as arguments, build and set them.
  /// @param[in] container Container which stores texture atlas information.
	///
	CTexture2DSprite(const resource::STexture2DAtlas& container);

	///
	/// @brief Create m_texture with no m_texture path, but for later use.
	/// @param[in] internal_format color format or internal format to set up each pixel.
	/// @param[in] format format of entire texture.
	/// @param[in] type type.
	/// @param[in] width texture width size to create.
	/// @param[in] height texture height size to create.
	///
	CTexture2DSprite(GLint internal_format, GLenum format, GLenum type, GLsizei width = 256, GLsizei height = 224);

	///
	/// @brief Set texture's rendering option. (one item version)
	/// @param[in] option Option of texture parameter, such as GL_TEXTURE_MIN_FILTER, etc
	/// @param[in] mode Mode for option of texture parameter, like a GL_NEAREST, GL_REPEAT
	///
	void SetTextureParameterI(const GLint option, const GLint mode);

	///
	/// @brief Set texture's rendering options. (multiple item version)
	/// @param[in] lists The list of texture parameters.
	///
	void SetTextureParameterI(const std::vector<TextureParameter>& lists);

	///
	/// @brief Set border color of texture.
	/// @param[in] border_color array of border color which consists of R, G, B, A.
	///
	void SetBorderColor(const std::array<GLfloat, 4>& border_color);

	///
  /// @brief Get Texture id
  ///
	int32_t Id() const noexcept {
    return m_texture_id;
  }

  ///
  /// @brief Get Texture overall width
  ///
  int32_t Width() const noexcept {
    return m_texture_width;
  }

  ///
  /// @brief Get Texture overall height
  ///
  int32_t Height() const noexcept {
    return m_texture_height;
  }

  ///
  /// @brief Get Texel information from m_texels with given index number.
  /// If given index number is out of bound, return nullptr.
  /// And is_texture_atlas is false so this texture is not concerned with atlas, return nullptr;
  ///
  std::optional<DVector2> GetTexelPtr(const ETexelType texel_type, int32_t index) const noexcept {
    if (m_texels && is_texture_atlas && index < m_texels->size()) {
      switch (texel_type) {
      case ETexelType::LeftDown: {
          const float (&xy)[2] = (*m_texels)[index].left_down;
          return DVector2{xy[0], xy[1]};
        }
      case ETexelType::RightUp: {
          const float (&xy)[2] = (*m_texels)[index].right_up;
          return DVector2{xy[0], xy[1]};
        }
      }
    }

    return std::nullopt;
  }

  ///
  /// @brief Return whether this texture handles atlas information or not.
  ///
  bool DoesHasAtlas() const noexcept {
    return is_texture_atlas;
	}

private:
	mutable GLuint m_texture_id = 0;
  int32_t m_texture_width  = 0;
  int32_t m_texture_height = 0;
  bool is_texture_atlas = false;

  const std::vector<resource::STexture2DTexelInformation>* m_texels = nullptr;
};

} /// ::opgs16::texture namespace

#endif /// OPGS16_SYSTEM_FRAME_TEXTURE_H