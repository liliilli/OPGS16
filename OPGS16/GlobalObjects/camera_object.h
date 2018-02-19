#ifndef OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H
#define OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H

/**
 * @file GlobalObjects/camera_object.h
 * @brief This file consists of Camera object, not Legacy Camera.
 *
 * @author Jongmin Yun
 * @date 2018-02-15
 */
#include "..\System\Object\object.h"

/**
 * @class MainCameraObject
 * @brief Refined camera object class, different legacy camera object is independent of m_object_list.
 * You can set it up between two types, perspective (view-frustum) and orthographic (view-cube).
 * If you see 3D world more realistic, set PERSPECTIVE. Otherwise if you set UI components with
 * no distortion, 3D world or 2D world with no sight distortion, set ORTHO.
 */
class MainCameraObject final : public Object {
public:
    MainCameraObject();
};

#endif /** OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H */

