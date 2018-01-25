#ifndef OPENGL_TUTORIAL_OBJECTS_TONE_MAP_H
#define OPENGL_TUTORIAL_OBJECTS_TONE_MAP_H
#include "..\Headers\common.h"

class ToneMap : public Object {
public:
    ToneMap();
    virtual ~ToneMap() = default;

private:
    texture::Texture2D m_tex_src;
};

#endif /** OPENGL_TUTORIAL_OBJECTS_TONE_MAP_H */