#ifndef OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_CANVAS_H
#define OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_CANVAS_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element\Canvas\canvas.h
/// @brief Canvas frame object to display UI components.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-19 Remove Draw(ShaderNew) obsolete method. Replace with Render(), LocalUpdate()
/// 2018-03-11 Refactoring.
/// 2018-04-16 Move ::canvas::CCanvas to ::opgs16::element::canvas::CCanvas.
/// 2018-04-17 Move file path to System/Element/Canvas/Public.
///

/// ::opgs16::element::UiObject
#include <Element/ui_object.h>
#include <opgs16fwd.h>

namespace opgs16::element::canvas {

///
/// @class CCanvas
/// @brief Canvas frame object saves and manages UI components and permit accessing from UI m_object_list.
///
/// This canvas offers several features. (and comming soon features)
/// 1. Each update frame, canvas updates orthographic projection size
/// 2. Each draw frame, canvas object calls UI components.
/// 3. UI components can access Canvas object (parent), get orthographic projection.
/// 4. when Canvas object is destroyed, children UI components are automatically disorganized.
///
/// @log
/// 2018-02-19 Remove Draw(ShaderNew) obsolete method. Replace with Render(), LocalUpdate()
/// 2018-04-16 Move ::canvas::CCanvas to ::opgs16::element::canvas::CCanvas.
///
class CCanvas : public opgs16::element::UiObject {
public:
	///
	/// @brief Canvas constructor.
	/// CCanvas has orthographic camera fixed to screen size,
  /// this camera will be initialized.
	///
	CCanvas();
	virtual ~CCanvas() = default;

	void LocalUpdate() override;

  /// Disable DEPTH_TEST until All UiObject rendering are done.
	void Render() override;

	/// Get Ui camera's orthographic projection matrix.
	const glm::mat4& GetUiCameraPVMatrix() const;

  const glm::mat4& GetUiCameraProjectMatrix() const noexcept;

  const glm::mat4& GetUiCameraViewMatrix() const noexcept;

private:
	/// Camera object uses just one orthographic type.
  component::CCamera* m_camera = nullptr;

	bool m_is_size_changed;
};

} /// ::opgs16::element::canvas namespace

#endif // OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_CANVAS_H