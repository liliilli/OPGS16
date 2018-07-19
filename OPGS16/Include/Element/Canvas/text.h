#ifndef OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_TEXT_H
#define OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_TEXT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Canvas/text.h
///
/// @brief
/// Text object used in Canvas hierarchy.
///
/// @author Jongmin Yun
/// @log
/// 2018-02-19 Remove Draw(ShaderNew) obsolete method. Replace Draw() with Render()
/// 2018-03-11 Correction of UiObject namespace hierarchy positioning, and path.
/// 2018-04-17 Move ::canvas::CText to ::opgs16::element::canvas::CText.
///

#include <glm/glm.hpp>

/// ::opgs16::element::interface::IAlignable
#include <Element/Interface/i_alignable.h>
/// ::opgs16::element::UiObject
#include <Element/ui_object.h>

/// Forward declaration
#include <opgs16fwd.h>
/// ::opgs16::DColor (float)
#include <Helper/Type/color.h>

namespace opgs16::component {

class CFont2DRenderer;

} /// ::opgs16::component namespace

namespace opgs16::element::canvas {
///
/// @class CText
/// @brief This class display text on position aligned with FontManager.
/// @log
/// 2018-04-17 Move ::canvas::CText to ::opgs16::element::canvas::CText.
/// 2018-05-28 Remove pointer to implementation idiom.
///
class CText : public UiObject, public IAlignable {
public:
  ///
  /// @brief Text component (stand-alone) constructor.
  /// In initialization time of this class, initial text and position and color set.
  /// position parameter was based on screen or parent's size where component
  /// is in hierarchy structrue of parent.
  ///
	CText(const std::string& initial_txt,
        const DVector3& position = DVector3{ 0.f },
        const DColor& color = DColor{ 1.f, 1.f, 1.f });

	virtual ~CText() = default;

	void Render() override final;

  ///
  /// @brief Set text string to display.
  ///
  /// @param[in] new_text New text string.
  ///
	void SetText(const std::string& new_text);

  ///
  /// @brief Get text string to display.
  ///
  /// @return Text string which this instance has.
  ///
  const std::string& GetText() const;

	///
	/// @brief Set font size of string.
  /// This method has side-effect for accessing FontManager.
  ///
	/// @param[in] size
	///
	void SetFontSize(const uint32_t size);

  ///
  /// @brief Get font size which this instance has.
  ///
  /// @return unsigned font size value.
  ///
  const unsigned GetFontSize() const;

	///
	/// @brief Set this object font to render with.
	/// If FontManager doesn't find font, return false to inform method was failed.
  ///
	/// @param[in] font_tag Font name tag.
  ///
	/// @return Success flag.
	///
	bool SetFontName(const std::string& font_tag);

  ///
  /// @brief
  ///
  /// @return
  ///
  const std::string& GetFontName();

  ///
  /// @brief Set string color, single color with composited of {r, g, b}.
  ///
  /// @param[in] color New color value with consists of {r, g, b} glm::vec3 vector.
  /// each value must be in range of [0, 1], otherwise clamped to 0 or 1.
  ///
  void SetColor(const DColor& color);

  ///
  /// @brief
  ///
  /// @return
  ///
  const DColor& GetColor();

  ///
  /// @brief
  ///
  void SetRenderingLayer(int32_t layer_index);

  ///
  /// @brief
  ///
  void SetRenderingLayer(const std::string& layer_string);

private:

  //! ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
  //! Member data.
  //! ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

  /// Text to display on screen.
  /// String must be following UTF-8 encoding.
  std::string m_text{};
  /// Font name to display what shapes has to be.
	std::string m_font_name{};
  /// Font color what color to be displayed.
	DColor m_color{};
  /// Font size how display size big is.
  uint32_t m_font_size{};

  opgs16::component::CFont2DRenderer* m_component = nullptr;
};
} /// ::opgs16::element::canvas namespace.

#endif /// OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_TEXT_H
