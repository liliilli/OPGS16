#include "start.h"
#include "..\application.h"
#include "..\Headers\canvas_components.h"

#include "..\GlobalObjects\camera_object.h"     /*! MainCameraObject */
#include "..\Objects\TempDebug\test_image.h"    /*! TestImage for branch feature_instantiate */
#include "..\Objects\TempDebug\test_start_txt.h"/*! TestStartTxt for feature_instantiate */
#include "..\Objects\TempDebug\test_start_t_copy.h" /*! TestStartTCopy for same branch. */
#include "..\Objects\Temporary\object_collision.h"  /*! ObjecCOllision */

void Start::Draw() {
    glClearColor(.2f, .0f, .0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void Start::Initiate() {
    auto main_camera = std::make_unique<MainCameraObject>();
    Instantiate("MainCamera", main_camera);

    auto canvas = std::make_unique<Canvas::Canvas>();
    canvas->Instantiate<TestStartTCopy>("Copyright");
	Instantiate("Canvas", canvas);

    auto test_obj = std::make_unique<ObjectCollidable>("Test");
    Instantiate("Object_1", test_obj);

    ///*! To call template function with specific type argument, must know complete information. */
    //canvas->Instantiate<TestImage>("Image", canvas); {
    //    auto image = canvas->GetChild("Image");
    //    image->Instantiate<TestStartTxt>("Txt"); {
    //        image->GetChild("Txt")->SetLocalPosition({ -48, 0, 0 });
    //    }
    //    /*!
    //     * @defect Get cloned instance's tag directly and change behavior is very dangerous.
    //     * If object's cloning mechanism is changed, this tag becomes to be worthless.
    //     */
    //    image->Instantiate<TestStartTxt>("Txt"); {
    //        image->GetChild("Txt_1")->SetLocalPosition({ -48, 72, 0 });
    //    }
    //    image->Instantiate<TestStartTxt>("Txt"); {
    //        image->GetChild("Txt_2")->SetLocalPosition({ -48, -72, 0 });
    //    }
    //}
}

void Start::Update() {
	Scene::Update();
}
