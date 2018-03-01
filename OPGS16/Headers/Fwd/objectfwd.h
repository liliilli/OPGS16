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

/*!
 * @file Header/Fwd/object_fwd.h
 * @brief Forward declaration list of framework.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-01 Added opgs16 namespace and elements.
 */

#include <glm/fwd.hpp>	/** glm matrix forward declaration. */

namespace opgs16 {
enum class ScaleType : unsigned;
struct  GlobalOption;
class   GlobalSetting;
}

namespace helper { class ShaderNew; }
namespace camera { class CameraObject; }
namespace texture { class Texture2D; }
namespace shading {
class PostProcessingManager;
}

struct	GLFWwindow;

class   Object;
class	ObjectImpl;
struct	ObjectImplDeleter { void operator()(ObjectImpl* p); };

struct	ObjectTree;

namespace resource {
enum class ScopeType;
struct  Texture2D;
}

// Derived
namespace canvas {
class   Canvas;
class   Text;
class   TextImpl;
struct  TextImplDeleter { void operator()(TextImpl* p); };
}

class   Scene;

class   SpriteRenderer;
class   SpriteRendererImpl;
struct  SpriteRendererImplDeleter { void operator()(SpriteRendererImpl* p); };

class   VertexArrayObject;
class   ShaderNew;
class   ShaderWrapper;

/** Managers */
class   FontManager;    /*! Font management */
class   InputManager;   /*! Input polling */
class   ObjectManager;  /*! Objects */
class   ResourceManager;/*! Resource */
class   PhysicsManager; /*! Physics & collision */
class   SceneManager;   /*! Scenes */
class   SoundManager;   /*! Sounds */
class   TimeManager;    /*! Tick time */
class   TimerManager;   /*! Timer */

class   TimerHandle;   /*! sub-type of timermanager */

/*! Custom Initiated Objects */
class   TestImage;      /*! TestImage : public Canvas::Image */
class   TestStartTxt;   /*! TestImage : public Canvas::Text */
class   TestStartTCopy; /*! TestStartTCopy : public Canvas::Text */

namespace component {
class Component;
class ScriptFrame;
class Rigidbody2D;
class Camera;
}

namespace collision {
class Collider2D;
class RectangleCollider2D;
}

#endif /** OPGS16_HEADERS_FWD_OBJECT_FWD_H */