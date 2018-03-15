#include "game_canvas.h"                        /*! Header file */
#include "../../../../GlobalObjects/Canvas/text.h" /*! Canvas::Text */
#include "../../Scripts/Test1/test_script_1.h"     /*! TestScript1 */
#include "../../Scripts/Test1/test_script_2.h"     /*! TestScript2 */
#include "../../../../System/Components/Public/empty_renderer.h"

GameCanvas::GameCanvas() {
    /*! Object binding */
    Instantiate<::canvas::Text>("Text", "Rotation, Scaling, Translation test.");
    auto text = static_cast<::canvas::Text*>(GetChild("Text")); {
        text->SetFontName("Solomon");
        text->SetFontSize(8);
        text->SetOrigin(IOriginable::Origin::DOWN_CENTER);
        text->SetAlignment(IAlignable::Alignment::CENTER);
        text->SetWorldPosition({ 0, 32, 0 });

        if (auto i = text->GetComponent<opgs16::component::CEmptyRenderer>(); i) {
            i->SetRenderLayer(4);
        }
    }

    AddComponent<TestScript1>(*this);
    AddComponent<TestScript2>(*this);
}
