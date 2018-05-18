#ifndef OPGS16_S_GLOBAL_OBJECTS_INTERFACE_I_ALIGNABLE_H
#define OPGS16_S_GLOBAL_OBJECTS_INTERFACE_I_ALIGNABLE_H

/**
 * @file GlobalObjects/Interface/i_alignable.h
 * @brief IAlignable, not-pure interface file.
 *
 * @author Jongmin Yun
 * @date 2018-02-08
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
	enum class Alignment : int {
        LEFT,   /** Alignable instance would be displayed to the point started from position. */
        CENTER, /** Alignable instance would be displayed to the position is center of instance. */
        RIGHT   /** Alignable instance would be displayed to the point ended to position. */
    };

	/**
	 * @brief Set Alignment value. This method will change display position of IAlignable instance.
	 * @param[in] value New alignment value.
	 */
     inline void SetAlignment(Alignment value) {
        alignment = value;
    }

	/**
	 * @brief Get Alignment value.
	 * @return Alignment type value.
	 */
    inline Alignment GetAlignment() const {
        return alignment;
    }

private:
	Alignment alignment{ Alignment::LEFT }; /** Private alignment data */
};

#endif /** OPGS16_S_GLOBAL_OBJECTS_INTERFACE_I_ALIGNABLE_H */
