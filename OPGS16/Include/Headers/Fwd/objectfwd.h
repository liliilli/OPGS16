#ifndef OPGS16_HEADERS_FWD_OBJECT_FWD_H
#define OPGS16_HEADERS_FWD_OBJECT_FWD_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file Header/Fwd/object_fwd.h
 * @brief Forward declaration list of framework.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-01 Added opgs16 namespace and elements.
 * 2018-03-07 Remove SpriteRenderImplDeleter. (Redundant)
 * 2018-04-16 Rename canvas::Canvas, Image, Text classes to CCanvas, CImage, CText.
 * 2018-04-17 Move ::canvas to ::opgs16::element::canvas namespace.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <glm/fwd.hpp>	/** glm matrix forward declaration. */

struct	GLFWwindow;
//struct	ObjectTree;

namespace opgs16 {
enum class EScaleType : unsigned;
class   SGlobalSetting;

namespace component {
class   CAnimator;
class   CCamera;
class   CRigidbody2D;
class   CScriptFrame;
class   CSprite2DRenderer;
class   CPreProcessingGetter;
class   CPreProcessing2DRenderer;

namespace _internal {
class   CComponent;
class   CSpriteRendererImpl;
} /*! opgs16::component::_internal */
} /*! opgs16::component */

namespace debug {
class   CLogger;
namespace _internal {
class   CLoggerImpl;
} /*! opgs16::debug::_internal */
} /*! opgs16::debug */

namespace element {
class   CObject;
class   CShaderNew;
class   CScene;
class   CTimerHandle;   /*! sub-type of timermanager */
class   CVertexArrayObject;
class   CShaderWrapper;
class   CFrameBuferFrame;
class   CPostProcessingFrame;
class   CPreRenderingFrame;
class   CPreRenderingContainer;

namespace _internal {
class	CObjectImpl;
} /*! opgs16::element::_inetrnal */

namespace canvas {
class   CCanvas;
class   CImage;
class   CText;

namespace _internal {
class   CTextImpl;
struct  TextImplDeleter { void operator()(CTextImpl* p); };

} /*! opgs16::element::canvas::_internal */
} /*! opgs16::element::canvas */
} /*! opgs16::element */

namespace manager {
class   MFontManager;    /*! Font management */
class   MInputManager;   /*! Input polling */
class   MObjectManager;  /*! Objects */
class   MPhysicsManager; /*! Physics & collision */
class   MPostProcessingManager;
class   MResourceManager;
class   MSceneManager;   /*! ScenesManager */
class   MSettingManager; /*! MSettingManager */
class   MSoundManager;   /*! Sounds */
class   MTimeManager;    /*! Tick time */
class   MTimerManager;   /*! Timer */
} /*! opgs16::manager */

namespace physics {
class   CCollider2D;
class   CRectangleCollider2D;

} /*! opgs16::namespace */

namespace builtin {
namespace shader {
class   SGlobalQuad2D;

} /*! opgs16::builtin::shader */
} /*! opgs16::builtin */

namespace resource {
enum class EShaderType : int;
struct  STexture2D;
struct  STexture2DAtlas;
class   SFont;
class   SSound;
class   SShader;
struct  SAnimation;
namespace _internal {
enum class ESymbolType;
enum class EResourceType;
} /*! opgs16::resource::_internal */
} /*! opgs16::resource */

namespace texture {
class   CTexture2D;
} /*! opgs16::texture */
} /*! opgs16 */

#endif /** OPGS16_HEADERS_FWD_OBJECT_FWD_H */