#ifndef OPGS16__MODEL_H
#define OPGS16__MODEL_H

#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <glm\glm.hpp>
#include <iostream>
//#include "helper.h"
#include "..\Shader\shader.h"

namespace model {

struct Vertex {
    glm::vec3   position;
    glm::vec3   normal;
    glm::vec2   texture_coords;
};

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    EMISSION,
    NORMAL,
    HEIGHT,
    REFLECTION
};

struct Texture {
    unsigned    id;
    TextureType type;
    std::string path;
};

/**
 * \brief   a storage for each mesh information of one model.
 * \details
 * \author  Jongmin Yun (Neu), jmyundev@gmail.com
 * \date    2017-12-26
 * \version 0.0.1
 */
class Mesh {
public:
    // Mesh Data
    std::vector<Vertex>     vertices;
    std::vector<unsigned>   indices;
    std::vector<Texture>    textures;

    // Functions
    Mesh(const std::vector<Vertex>& vetices,
         const std::vector<unsigned>& indices,
         const std::vector<Texture>& textures);
    Mesh(std::vector<Vertex>&& vetices,
         std::vector<unsigned>&& indices,
         std::vector<Texture>&& textures);

    /**
     * \brief
     * \details
     * \param
     * \return  void
     */
    void SetTranslationMatrix();

    /**
     * \brief   Draw model object with iterative mesh's draw call.
     * \details Draw model object onto screen or off-screen rendering. What to draw on is
     * depending on frame buffer on using now. this methods could be combined with Mesh::
     * SetTranslationMatrix() used for setting instanced rendering.
     * \param   helper::Shader& shader, const unsigned amount = 1
     * \return  void
     *
     * \see     https://learnopengl.com/#!Model-Loading/Model
     */
    void Draw(helper::Shader& shader, const unsigned amount = 1) const;

private:
    // Render Data
    unsigned VAO, VBO, EBO;

    // VBO and EBO byte size to be used in SetupMesh()
    const unsigned VBO_SIZE;
    const unsigned EBO_SIZE;

    // Functions
    void SetupMesh();
    std::string GetVariableName(const std::string& lhs, const unsigned id) const {
        return (lhs + std::to_string(static_cast<int>(id)));
    }
};

/**
 * \brief   The class represents a model information entities.
 * \details Model class is storage for model information related to meshes, nods
 *          es, etc. And you could draw model with Draw function with shader you
 *          want to do with.
 * \author  Jongmin Yun (Neu), jmyundev@gmail.com
 * \date    2017-12-26
 * \version 0.0.1
 */
class Model {
public:
    Model(std::string path);

    void Draw(helper::Shader& shader, const unsigned amount = 1) const;

    template <typename _Ty, size_t _Size>
    void SetVAOTranslation(std::array<_Ty, _Size>& matrix_buffer);

private:
    // Model Data
    std::vector<Texture> loaded_textures;
    std::vector<Mesh> meshes;
    std::string directory;

    /**
     * \brief
     * \details
     * \param
     * \return  void
     *
     * \see     https://learnopengl.com/#!Model-Loading/Model
     */
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* const node, const aiScene* const scene);
    Mesh ProcessMesh(aiMesh* const mesh, const aiScene* const scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial* const mat, aiTextureType type,
                                              TextureType tex_type);
};

template<typename _Ty, size_t _Size>
inline void Model::SetVAOTranslation(std::array<_Ty, _Size>& matrix_buffer) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, matrix_buffer.size() * sizeof(glm::mat4), &matrix_buffer[0],
                 GL_STATIC_DRAW);

    for (auto& mesh : meshes) {
        mesh.SetTranslationMatrix();
    }
}

}

#endif
