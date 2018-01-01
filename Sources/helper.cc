#include "helper.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\type_ptr.hpp>
#include "stb_image.h"

namespace helper {

Shader::Shader(const GLchar * vertex_path, const GLchar * fragment_path) {
    // Retrieve the vertex / fragment source code from each file path
    std::string vertex_code;
    std::string fragment_code;

    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    // Ensure ifstream objects can throw exceptions
    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open Files
        vertex_shader_file.open(vertex_path);
        fragment_shader_file.open(fragment_path);

        std::stringstream vertex_shader_stream;
        std::stringstream fragment_shader_stream;

        // Read file and bring to each stream
        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        // Close file handlers
        vertex_shader_file.close();
        fragment_shader_file.close();

        // Convert stream into plain text string
        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure err) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ" << std::endl;
    }

    // Setting
    auto vertex_shader_code = vertex_code.c_str();
    auto fragment_shader_code = fragment_code.c_str();

    // Log buffer setting to check compilation result of each shader program.
    int success = 0;
    char info_log[LOG_SIZE];

    // Vertex Shader compile
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    {
        glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
        glCompileShader(vertex_shader);

        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) ShaderErrorPrint(vertex_shader, info_log);
    }

    // Fragment Shader compile
    GLuint fragment_shader;
    {
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);
        glCompileShader(fragment_shader);

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) ShaderErrorPrint(fragment_shader, info_log);
    }

    // Shader program object creation and linking
    kId = glCreateProgram();
    {
        glAttachShader(kId, vertex_shader);
        glAttachShader(kId, fragment_shader);
        glLinkProgram(kId);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        glGetShaderiv(kId, GL_COMPILE_STATUS, &success);
        if (!success) ShaderErrorPrint(kId, info_log);
    }
}

void Shader::Use() {
    // Just use kId to load associated with shader program object.
    glUseProgram(kId);
}

void Shader::SetBool(const std::string & name, bool value) const {
    glUniform1i(glGetUniformLocation(kId, name.c_str()), static_cast<GLint>(value));
}

void Shader::SetInt(const std::string & name, int value) const {
    glUniform1i(glGetUniformLocation(kId, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, float value) const {
    glUniform1f(glGetUniformLocation(kId, name.c_str()), value);
}

void Shader::SetVec3f(const std::string & name,
                      const float _1, const float _2, const float _3) {
    glUniform3f(glGetUniformLocation(kId, name.c_str()), _1, _2, _3);
}

void Shader::SetVec3f(const std::string& name, const glm::vec3& vector) {
    glUniform3fv(glGetUniformLocation(kId, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::SetVecMatirix4f(const std::string& name,
                             glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(kId, name.c_str()),
                       1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderErrorPrint(GLuint shader, char* info_log) {
    glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
}

/**
 * Texture Mode
 */
Texture2D::Texture2D(const GLchar * texture_path, const GLint bind_mode) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set the texture wrapping and filtering options
    // Load image file to use as texture image.
    auto width = 0;
    auto height = 0;
    auto nr_channels = 0;
    auto data = stbi_load(texture_path, &width, &height, &nr_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, bind_mode, width, height, 0,
                     bind_mode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "FAILED::LOAD::TEXTURE" << std::endl;
    }
    stbi_image_free(data);

}

void Texture2D::SetTextureParameterI(const GLint option, const GLint mode) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, option, mode);
}

void Texture2D::SetTextureParameterI
(std::initializer_list<std::pair<_Option, _Mode>> lists) {

    glBindTexture(GL_TEXTURE_2D, texture);
    for (const auto& option_item : lists) {
        glTexParameteri(GL_TEXTURE_2D, option_item.first, option_item.second);
    }
}

}