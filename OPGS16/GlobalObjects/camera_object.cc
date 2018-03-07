#include "camera_object.h"                      /*! Header file */
#include "../System/Components/Public/camera.h" /*! component::Camera */

MainCameraObject::MainCameraObject() {
    using Camera = opgs16::component::Camera;
    AddComponent<Camera>(*this, Camera::ViewType::ORTHO, Camera::CameraType::MAIN);
}
