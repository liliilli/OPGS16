#include "camera_object.h"                      /*! Header file */
#include "../System/Components/Public/camera.h" /*! component::CCamera */

MainCameraObject::MainCameraObject() {
    using Camera = opgs16::component::CCamera;
    AddComponent<Camera>(*this, Camera::ViewType::ORTHO, Camera::CameraType::MAIN);
}
