#include "game_canvas.h"                        /*! Header file */
#include "..\..\..\GlobalObjects\Canvas\text.h" /*! Canvas::Text */
#include "..\Scripts\test_script_1.h"           /*! TestScript1 */

GameCanvas::GameCanvas() {
    /*! Object binding */
    Instantiate<::Canvas::Text>("Text", "Rotation, Scaling, Translation test.");
    auto text = static_cast<::Canvas::Text*>(GetChild("Text")); {
        text->SetFontName("Solomon");
        text->SetFontSize(8);
        text->SetOrigin(IOriginable::Origin::DOWN_CENTER);
        text->SetAlignment(IAlignable::Alignment::CENTER);
        text->SetWorldPosition({ 0, 32, 0 });
    }

    AddComponent<TestScript1>(*this);
}