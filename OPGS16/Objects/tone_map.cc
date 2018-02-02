#include "tone_map.h"

ToneMap::ToneMap() :
    m_tex_src{ texture::Texture2D{ "Resources/california.jpg", GL_RGB } } {

}
