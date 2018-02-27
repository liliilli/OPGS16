#include "game_canvas_2.h"                      /*! Header file */
#include "..\..\..\GlobalObjects\Canvas\text.h" /*! Canvas::Text */
#include <limits>
#include <cmath>

namespace {
constexpr float pi2_div_64 = 3.1415926535 * 2 / 64.0f;
}

GameCanvas2::GameCanvas2() {
    /*! Object binding */
    auto interval = 8u;
    glm::vec3 position{ 0, 224 - interval, 0 };
    glm::vec3 color{ 1, 1, 1 };

    char character = 0;
    auto index = 0u;
    std::string font_string = "Solomon";

    for (; position.y >= 0; position.y -= interval) {
        for (position.x = 0; position.x <= 256; position.x += interval) {
            auto text = std::make_unique<canvas::Text>(std::string{ character });
            text->SetWorldPosition(position);
            text->SetOrigin(IOriginable::Origin::DOWN_LEFT);
            text->SetFontName(font_string);
            text->SetFontSize(8);
            text->SetColor(GetColor(index));

            Instantiate<canvas::Text>("Text", text);

            /*! Increase character */
            ++character;
            ++index;
            if (character == std::numeric_limits<char>::min())
                character = 0;
        }

        if (position.y == 112)
            font_string = "BIOS";
    }
}

glm::vec3 GameCanvas2::GetColor(unsigned index) {
    auto r = std::cosf(pi2_div_64 * index) / 2 + 0.5f;
    auto g = std::cosf(pi2_div_64 * (index + 2)) / 2 + 0.5f;
    auto b = std::cosf(pi2_div_64 * (index + 4)) / 2 + 0.5f;
    return glm::vec3{ r, g, b };
}
