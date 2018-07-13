#ifndef OPGS16_SYSTEM_OBJECT_UI_OBJECT_H
#define OPGS16_SYSTEM_OBJECT_UI_OBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/ui_object.h
/// @brief Base clase for m_object_list inherit UiObject. (UI component)
///
/// @author Jongmin Yun
/// @log
/// 2018-02-19 Remove Draw() override and Draw(ShaderNew) deleted method.
/// 2018-03-11 Correction of UiObject namespace hierarchy positioning, and path.
/// 2018-07-02 Refactring.
///

#include <GL/glew.h>
/// ::opgs16::element::CObject
#include <Element/object.h>
/// ::opgs16::IOriginable
#include <Element/Interface/i_originable.h>

namespace opgs16::element {

///
/// @class UiObject
/// @brief UiObject class would be used to implement Ui component, such as Canvas, Image, etc.
/// First, UiObject has slightly different Update mechanism, to display components hierarcherly.
///
/// Second, UiObject has only a child inherits UiObject class.
/// Otherwise, undefined behavior will occur (I swear)
///
class UiObject : public CObject, public IOriginable {
public:
	virtual ~UiObject() = default;

  /// Uiobject updates children edge position.
  /**
	 * @brief Image instance updates x, y, w, h for each frame, referencing with Scale values.
	 * And, updates children calling Parent's Update method.
	 *
	 * This virtual methods can not override any more.
	 */
  void LocalUpdate() override;

protected:
	///
	/// @brief Updates screen_x, screen_y, width, height coordinate value
  /// in Screen space.
	/// This methods updates caller's screen_x, y, width, height itself.
  ///
	/// @param[in] parent_xywh Caller's screen_x,y,width,height position array.
	///
	void UpdateScreenXYWH(const std::array<GLint, 4> parent_xywh);

private:
  DVector2 screen_origin = DVector2{};
  DVector2 screen_size = DVector2{};

private:
	///
	/// @brief Updates callee(child)'s final position to be rendered.
	///
	/// This method gets parent(Caller)'s x, y, width, height value of Screen space,
	/// and calculate rendering position referring to IOriginable::Origin value of child.
	/// Detail mechanism is below.
	/// \(7)|(8)/(9) At first, method gets Origin value of child which is left and calculate
	/// -(4).(5)-(6) with parent_x, y, width, height to be aligned Parent's region without
	/// /(1)|(2)\(3) applying child's local position.
	/// Afterward, this method sets final rendering position of child and apply changed value to
	/// children of child calling SetUiParentPosition() of children of child.
	///
	void SetUiParentPosition(float parent_x, float parent_y,
                           float parent_width, float parent_height);
};

} /// ::opgs16::element namespace

#endif /// OPGS16_SYSTEM_OBJECT_UI_OBJECT_H
