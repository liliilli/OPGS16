#ifndef OPGS16_CANVAS_IMPL_TEXT_IMPL_H
#define OPGS16_CANVAS_IMPL_TEXT_IMPL_H

#include <string>       /** std::string */
#include <glm\glm.hpp>  /** glm::vec3 */
#include "..\..\..\Headers\Fwd\objectfwd.h" /** Canvas::Text */

namespace Canvas {

class TextImpl final {
public:
    explicit TextImpl(Text&);

    inline void SetText(const std::string& new_text) {
        m_text = new_text;
    }
    inline const std::string& GetText() const {
        return m_text;
    }

    void SetFontName(const std::string& font_name) {
        m_font_name = font_name;
    }
    inline const std::string& GetFontName() const {
        return m_font_name;
    }

    void SetFontSize(const unsigned size);
    inline const unsigned GetFontSize() {
        return m_font_size;
    }

    void SetColor(const glm::vec3& color);
    void SetColor(const float r, const float g, const float b);
    inline const glm::vec3 GetColor() const {
        return m_color;
    }

private:
	std::string m_text{};       // Text to display on screen.
	std::string m_font_name{};  // Font name to display what shapes has to be.
    unsigned m_font_size{};     // Font size how display size big is.
	glm::vec3 m_color{};        // Font color what color to be displayed.

    Text& m_handle_ref;         // Reference of Text handle.
};

}

#endif // !OPGS16_CANVAS_IMPL_TEXT_IMPL_H
