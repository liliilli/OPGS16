#ifndef OPENGL_TUTORIAL_HELPER_H
#define OPENGL_TUTORIAL_HELPER_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

namespace helper {
constexpr unsigned LOG_SIZE = 0x200;

class Shader {
public:
    // Costructor reads vertex shader and fragment shader path, and build them.
    Shader(const GLchar* vertex_path, const GLchar* fragment_path);

    // Use / Activate the shader
    void Use();
    // Utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;

    void SetVec3f(const std::string& name, const float, const float, const float);
    void SetVec3f(const std::string & name, const glm::vec3& vector);
    void SetVecMatirix4f(const std::string& name, glm::mat4& matrix);

    const GLuint GetId() const { return kId; }

private:
    // The program ID
    mutable GLuint kId;

    void ShaderErrorPrint(GLuint shader, char* info_log);
};

class Texture2D {
private:
    using _Option = GLint;
    using _Mode = GLint;

public:
    // Constructor get path and texture mode as arguments, build and set them.
    Texture2D(const GLchar* texture_path, const GLint bind_mode);
    void SetTextureParameterI(const GLint option, const GLint mode);
    void SetTextureParameterI(std::initializer_list<std::pair<_Option, _Mode>> lists);

    const GLuint GetId() const { return texture; }

private:
    mutable GLuint texture;
};

}