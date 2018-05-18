#ifndef OPGS16_SYSTEM_MANAGER_PRERENDERING_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_PRERENDERING_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

///
/// @file Manager/prerendering_manager.h
/// @brief Management class file of pre-rendering frame classes.
/// @log
/// 2018-04-21 Create file.
/// 2018-04-29 Add IsFrameBufferExist(const char*) function.
///

/// ::opgs16::element::CFrameBufferFrame
#include <Shader\framebuffer.h>
/// ::opgs16::element::CPreRenderingContainer
#include <Element\prerendering_container.h>

namespace opgs16::manager::prerendering {

/*!
 * @brief Find whether pre-processing frame buffer which has name frame_buffer_name is exist or not.
 * @return If found return true, or false.
 */
bool IsFrameBufferExist(const char* frame_buffer_name) noexcept;

/*!
 * @brief Generate
 */
element::CPreRenderingContainer*
GenerateFrameBuffer(const char* frame_buffer_name);

/*!
 * @brief Get
 */
element::CPreRenderingContainer* GetFrameBuffer(const char* frame_buffer_name) noexcept;

/*!
 * @brief Update
 */
void Update();

/*!
 * @brief Render
 */
void Render();

} /*! opgs16::manager::prerendering */

#endif // OPGS16_SYSTEM_MANAGER_PRERENDERING_MANAGER_H
