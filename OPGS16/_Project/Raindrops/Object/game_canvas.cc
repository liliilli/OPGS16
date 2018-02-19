#include "game_canvas.h"                        /*! Header file */
#include "..\..\..\GlobalObjects\Canvas\text.h" /*! Canvas::Text */
#include "..\Scripts\rain_ui.h"                 /*! RainUiManager */

GameCanvas::GameCanvas() {
    /*! Object binding */
    Instantiate<::Canvas::Text>("Score", "");
    auto score = static_cast<::Canvas::Text*>(GetChild("Score"));
    score->SetFontName("Solomon");
    score->SetFontSize(8);
    score->SetOrigin(IOriginable::Origin::UP_LEFT);
    score->SetWorldPosition({ 16, -16, 0 });

    AddComponent<RainUiManager>(*this, score);
}