#ifndef OPGS16_GLOBAL_OBJECTS_INTERFACE_I_ORIGINABLE_H
#define OPGS16_GLOBAL_OBJECTS_INTERFACE_I_ORIGINABLE_H

/**
 * @file GlobalObjects/Interface/i_originable.h
 * @brief IOriginable, not-pure interface file.
 *
 * @author Jongmin Yun
 * @date 2018-02-08
 */

/**
 * @class IOriginable
 * @brief This interface (not-pure, but work as a UI component checking type in RTTI and so on)
 * works as checking components type, get objects to align with Origin which is used for calcula
 * ting weight-center position of each object.
 */
class IOriginable {
public:
	/**
	 * @brief This enum is need for calculating originable instance's final rendering position.
     * IOriginable instance has a feature of calculating final poisiton mechanism with UiObject,
     * or FontManager.
     *
	 * \(7)|(8)/(9) At first, method gets Origin value of child which is left and calculate
	 * -(4).(5)-(6) with parent_x, y, width, height to be aligned Parent's region without
	 * /(1)|(2)\(3) applying child's local position.
     *
	 * Afterward, this method sets final rendering position of child and apply changed value to
	 * children of child calling SetUiParentPosition() of children of child.
	 */
	enum class Origin : int {
		UP_LEFT = 7,		UP_CENTER = 8,		UP_RIGHT = 9,
		CENTER_LEFT = 4,	CENTER_CENTER = 5,	CENTER_RIGHT = 6,
		DOWN_LEFT = 1,		DOWN_CENTER = 2,	DOWN_RIGHT = 3
	};

	/**
	 * @brief Set origin type value newly. Calling this does change rendering position of this.
	 * @param[in] value New origin value.
	 */
	 inline void SetOrigin(Origin value) {
		m_origin = value;
	}

	/**
	 * @brief Get origin type value.
	 * @return Origin type value.
	 */
    inline Origin GetOrigin() const {
        return m_origin;
    }

private:
	Origin m_origin{ Origin::CENTER_CENTER };   /** Private origin value */
};

#endif /** OPGS16_GLOBAL_OBJECTS_INTERFACE_I_ORIGINABLE_H */
