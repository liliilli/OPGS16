#include "start.h"
#include "..\application.h"
#include "..\Headers\canvas_components.h"

#include "..\GlobalObjects\camera_object.h"     /*! MainCameraObject */
#include "..\Objects\TempDebug\test_start_t_copy.h" /*! TestStartTCopy for same branch. */

#include "..\_Project\CollisionTest\Object\object_collision.h"  /*! ObjectCollidable */
#include "..\_Project\Raindrops\Object\rain_manager.h"  /*! RainManager */
//#include "..\Objects\TempDebug\test_image.h"    /*! TestImage for branch feature_instantiate */
//#include "..\Objects\TempDebug\test_start_txt.h"/*! TestStartTxt for feature_instantiate */
//#include "..\Objects\Temporary\object_collision.h"  /*! ObjecCOllision */
//#include "..\Objects\Temporary\random_moving.h"     /*! RandomMoving */

void Start::Draw() {
    glClearColor(.1f, .1f, .1f, 1.f);
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
    test_obj->SetFinalPosition(glm::vec3{ 128, 112,0 });
    test_obj->SetTag("Player");
    Instantiate("Object_1", test_obj);
    GetObject("Object_1")->SetFinalPosition(glm::vec3{ 128, 112, 0 });

    auto rain = std::make_unique<RainManager>();
    Instantiate("RainManager", rain);
    //{
    //    auto subject = std::make_unique<RandomMoveObject>("Test");
    //    subject->SetTag("Enemy");
    //    Instantiate("Rd1", subject);
    //    GetObject("Rd1")->SetFinalPosition(glm::vec3{ 224, 192, 0 });
    //}

    //{
    //    auto subject = std::make_unique<RandomMoveObject>("Test");
    //    subject->SetTag("Enemy");
    //    Instantiate("Rd2", subject);
    //    GetObject("Rd2")->SetFinalPosition(glm::vec3{ 73, 171, 0 });
    //}

    //{
    //    auto subject = std::make_unique<RandomMoveObject>("Test");
    //    //subject->SetTag("Enemy");
    //    Instantiate("Rd3", subject);
    //    GetObject("Rd3")->SetFinalPosition(glm::vec3{ 96, 64, 0 });
    //}

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
