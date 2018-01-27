#ifndef OPENGL_TUTORIALS_GLOBAL_OBJECTS_INTERFACE_I_ALIGNABLE_H
#define OPENGL_TUTORIALS_GLOBAL_OBJECTS_INTERFACE_I_ALIGNABLE_H

/**
 * @file GlobalObjects/Interface/i_alignable.h
 * @brief IAlignable not-pure interface file.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

/**
 * @interface IAlignable
 * @brief This non-pure interface are used in font alignment and font-manager to reference it.
 * The other class can also use it, but a programmer has to follow these laws.
 *
 * 1. If alignment is LEFT, rendering image must be displayed after barycentric position of object.
 * 2. If alignment is CENTER, halves of image must be displayed after/before position of object.
 * 3. If alignment is RIGHT, all image must be displayed before barycentric position of object.
 */
class IAlignable {
public:
	enum class Alignment : int { LEFT, CENTER, RIGHT };

	/**
	 * @brief
	 * @param[in] value
	 */
	[[noreturn]] inline void SetAlignment(Alignment value);

	/**
	 * @brief
	 * @return Alignment type value.
	 */
	inline Alignment GetAlignment() const;

private:
	Alignment alignment{ Alignment::LEFT };
};

inline void IAlignable::SetAlignment(IAlignable::Alignment value) {
	alignment = value;
}

inline IAlignable::Alignment IAlignable::GetAlignment() const {
	return alignment;
}

#endif /** OPENGL_TUTORIALS_GLOBAL_OBJECTS_INTERFACE_I_ALIGNABLE_H */
