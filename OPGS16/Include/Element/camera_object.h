#ifndef OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H
#define OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H

/**
 * @file GlobalObjects/camera_object.h
 * @brief This file consists of Camera object, not Legacy Camera.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-15 Create file and implement basic features.
 * 2018-03-11 Correction of object namespace position and path.
 */

#include <Element\object.h> /// ::opgs16::element::CObject

/**
 * @class MainCameraObject
 * @brief Refined camera object class, different legacy camera object is independent of m_object_list.
 * You can set it up between two types, perspective (view-frustum) and orthographic (view-cube).
 * If you see 3D world more realistic, set PERSPECTIVE. Otherwise if you set UI components with
 * no distortion, 3D world or 2D world with no sight distortion, set ORTHO.
 */
class MainCameraObject final : public opgs16::element::CObject {
public:
    MainCameraObject();
};

#endif /** OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H */

