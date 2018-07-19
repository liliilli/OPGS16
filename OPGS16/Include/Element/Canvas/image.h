#ifndef OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_IMAGE_H
#define OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_IMAGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element\Canvas\image.h
///
/// @brief Image object used in Canvas hierarchy.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-19 Remove Draw(ShaderNew) obsolete method.
///            Replace Draw(), Update() with Render() and LocalUpdate()
/// 2018-03-11 Correction of UiObject namespace hierarchy positioning, and path.
/// 2018-04-17 Move canvas::CImage into opgs16::element::canvas::CImage.
///

#include <Element/ui_object.h>  /// ::opgs16::element::UiObject
#include <opgs16fwd.h>          /// Forward declaration

namespace opgs16::element::canvas {
///
/// @class CImage
/// @brief UI CImage to display one patched image.
///
/// This class are able to have children, but only UiObject. unless, Run-time error will be invoked.
/// Actual display position of children(UiImage) will be constrained by Image's center position and
/// size.
///
class CImage : public opgs16::element::UiObject {
public:
	CImage(const std::string& sprite_tag, const CCanvas* const ref_canvas);
	CImage(const std::string& sprite_tag, const std::unique_ptr<CCanvas>& ref_canvas);

	virtual ~CImage() = default;

  void LocalUpdate() override;

	///
	/// @brief This calls callee to draw or render something it has.
	/// Afterward, it draws children calling Parent's Draw (no parameter) method.
	///
	/// This virtual methods can not override any more.
	///
	void Render() override final;

  void SetImageSize(const float width, const float height);

  void SetTextureFragmentIndex(const int32_t index);

  void SetRenderingLayer(const std::string& layer_name);

  void SetRenderingLayer(int32_t layer_value);

private:
  component::CSprite2DRenderer* m_renderer_ptr = nullptr;

  /// l_value reference of canvas to get projection matrix.
  CCanvas* const m_ref_canvas;
};

} /// ::opgs16::element::canvas namespace

#endif /// OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_IMAGE_H
