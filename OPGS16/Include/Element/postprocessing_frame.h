#ifndef OPGS16_SYSTEM_ELEMENT_PUBLIC_POSTPROCESSING_FRAME_H
#define OPGS16_SYSTEM_ELEMENT_PUBLIC_POSTPROCESSING_FRAME_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/// @file Element/postprocessing_frame.h
/// @brief Elementary post-processing frame to manage a thing to be rendered.
/// This file consists of application operation class and member API functions.
///
/// @author Jongmin Yun
/// @log
/// 2018-04-21 Create file.

#include <Shader\framebuffer.h> /// ::opgs16::element::CFrameBufferFrame

namespace opgs16::element {

/// @class CPostProcessingFrame
/// This class's m_object_list has frame buffer, texture, 
/// and render buffer to be able to bind frame buffer object.
/// And PostProcessingFrame has also shaders to render post-processing effects.
///
/// Each frame is managed by PostProcessingManager, 
/// you can call and bind it through Manager. But you should not try to bind 
/// this itself, not calling PostProcessingManager's BindSequence().
/// This will crash application or at least it incurs undefined behavior.
///
/// @log
/// 2018-04-21 Create file.
class CPostProcessingFrame : public CFrameBufferFrame {
public:
    /** Check this effect is on effect-sequences. */
    bool IsActive() const noexcept {
	    return (m_active_count != 0);
	}

	/** Set this effect will be actived. */
	void Active() noexcept {
	    m_active_count += 1;
	};

	/** Set this effect disabled. */
	void Disable() noexcept {
        if (IsActive()) m_active_count -= 1;
	}

private:
	unsigned m_active_count{ 0 };

};

} /*! ::opgs16::element */

#endif // OPGS16_SYSTEM_ELEMENT_PUBLIC_POSTPROCESSING_FRAME_H