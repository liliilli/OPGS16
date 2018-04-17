#ifndef OPGS16_OBJECTS_DB_DEBUG_CANVAS_H
#define OPGS16_OBJECTS_DB_DEBUG_CANVAS_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file Object\Debug\debug_canvas.h
 * @brief Canvas frame object to display UI components.
 *
 * @author Jongmin Yun
 * @date 2018-02-13
 * @log
 * 2018-02-13 Create file.
 * 2018-04-17 Change path of ::opgs16::element::canvas::CCanvas.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../System/Element/Canvas/Public/canvas.h"  /*! ::opgs16::element::canvas::CCanvas. */
#include "../../Headers/Fwd/objectfwd.h"                /*! component::CScriptFrame */

/*!
 * @class CanvasDebug
 * @brief temporary
 */
class CanvasDebug : public opgs16::element::canvas::CCanvas {
public:
	/**
	 * @brief Canvas constructor.
	 * Canvas has orthographic camera fixed to screen size, this camera will be initialized.
	 */
	CanvasDebug();
	virtual ~CanvasDebug() = default;
};

#endif /** OPGS16_OBJECTS_DB_DEBUG_CANVAS_H */