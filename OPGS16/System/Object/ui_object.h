#ifndef OPENGL_TUTORIALS_SYSTEM_FRAME_UI_OBJECT_H
#define OPENGL_TUTORIALS_SYSTEM_FRAME_UI_OBJECT_H

/**
 * @file System\Frame\ui_object.h
 * @brief Base clase for objects inherit UiObject. (UI component)
 *
 * @author Jongmin Yun
 */

#include <GL\glew.h>
#include "object.h"
#include "..\..\GlobalObjects\Interface\i_originable.h"

/**
 * @class UiObject
 * @brief UiObject class would be used to implement Ui component, such as Canvas, Image, etc.
 * First, UiObject has slightly different Update mechanism, to display components hierarcherly.
 *
 * UiObject does not implement Draw(helper::ShaderNew&) because of overriden version is
 * deprecated. If derived object inherits UiObject calls Draw(helper::ShaderNew&),
 * undefined behavior will be occured.
 *
 * So, All of derived objects are prohibited for overriding Draw(helper::ShaderNew&).
 *
 * Second, UiObject has only a child inherits UiObject class.
 * Otherwise, undefined behavior will occur (I swear)
 */
class UiObject : public Object, public IOriginable {
public:
	virtual ~UiObject() = default;

	/**
	 * @brief UiObject regards children as UiObject, and updates them.
	 * 1. UiObject updates children's rendering position with parent's screen_x, y, width, height.
	 * 2. UiObject updates children itself.
	 *
	 * This update methods will be overriden by derived class.
	 */
	[[noreturn]] virtual void Update() override;

	/**
	 * @brief Base UiObject class just call children's Draw() method.
	 * Children inherits UiObject must implement its own Draw() method, to render itself.
	 * And at last, it should call UiObject::Draw() to call rendering of children recursively.
	 *
	 * This update methods will be overriden by derived class.
	 */
	[[noreturn]] virtual void Draw() override;

protected:
	/**
	 * @brief Updates screen_x, screen_y, width, height coordinate value in Screen space.
	 * This methods updates caller's screen_x, y, width, height itself.
	 *
	 * @param[in] parent_xywh Caller's screen_x,y,width,height position array.
	 */
	[[noreturn]] void UpdateScreenXYWH(const std::array<GLint, 4> parent_xywh);

private:
	float screen_x{}, screen_y{};
	float screen_width{}, screen_height{};

private:
	/**
	 * @brief Updates callee(child)'s final position to be rendered.
	 *
	 * This method gets parent(Caller)'s x, y, width, height value of Screen space,
	 * and calculate rendering position referring to IOriginable::Origin value of child.
	 * Detail mechanism is below.
	 * \(7)|(8)/(9) At first, method gets Origin value of child which is left and calculate
	 * -(4).(5)-(6) with parent_x, y, width, height to be aligned Parent's region without
	 * /(1)|(2)\(3) applying child's local position.
	 * Afterward, this method sets final rendering position of child and apply changed value to
	 * children of child calling UpdateFinalPosition() of children of child.
	 */
	[[noreturn]] void UpdateFinalPosition(
		const float parent_x,
		const float parent_y,
		const float parent_width,
		const float parent_height);
};

#endif /** OPENGL_TUTORIALS_SYSTEM_FRAME_UI_OBJECT_H */