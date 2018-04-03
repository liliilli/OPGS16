#include "Model.h"

#include <stdexcept>
#include "..\..\__ThirdParty\stb\stb_image.h"

namespace model {
#define SIZE_OF(size, type) (void*)(size * sizeof(type))
#define SIZE_NULL (void*)0

Mesh::Mesh(const std::vector<Vertex>& _vertices,
           const std::vector<unsigned>& indices,
           const std::vector<Texture>& textures) :
    vertices{ _vertices },
    indices { indices },
    textures{ textures },
    VBO_SIZE{ static_cast<size_t>(_vertices.size()) * sizeof(Vertex) },
    EBO_SIZE{ static_cast<size_t>(indices.size()) * sizeof(size_t) } {

    // Setup
    SetupMesh();
}

Mesh::Mesh(std::vector<Vertex>&& _vertices,
           std::vector<unsigned>&& indices,
           std::vector<Texture>&& textures) :
    vertices{ _vertices },
    indices { indices },
    textures{ textures },
    VBO_SIZE{ this->vertices.size() * sizeof(Vertex) },
    EBO_SIZE{ this->indices.size() * sizeof(unsigned) } {

    // Setup
    SetupMesh();
}

void Mesh::SetTranslationMatrix() {
    glBindVertexArray(VAO);

    // vertex Attributes
    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void Mesh::SetupMesh() {
    // Generate and Bind vertex array object which is used for be-lightened object.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    // Bind vertex data to be used into Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, VBO_SIZE, &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBO_SIZE, &indices[0], GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), SIZE_NULL);
    glEnableVertexAttribArray(0);
    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));
    glEnableVertexAttribArray(2);

    //// vertex tangent
    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    //// vertex bitangent
    //glEnableVertexAttribArray(4);
    //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

void Mesh::Draw(helper::Shader& shader, const unsigned amount) const {
    auto diffuse_num    = 0u;
    auto specular_num   = 0u;
    auto emission_num   = 0u;
    auto reflection_num = 0u;

    using namespace std::string_literals;
    const auto diffuse_str  = "texture_diffuse"s;
    const auto specular_str = "texture_specular"s;
    const auto emission_str = "texture_emission"s;
    const auto reflection_str = "texture_reflection"s;

    for (auto i = 0u; i < textures.size(); ++i) {
        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);

        // Check each texture's type, and retrieve proper texture name to bind
        auto texture_name = ""s;
        switch (textures[i].type) {
        case TextureType::DIFFUSE:
            ++diffuse_num;
            texture_name = GetVariableName(diffuse_str, diffuse_num);
            break;
        case TextureType::SPECULAR:
            ++specular_num;
            texture_name = GetVariableName(specular_str, specular_num);
            break;
        case TextureType::EMISSION:
            ++emission_num;
            texture_name = GetVariableName(emission_str, emission_num);
            break;
        case TextureType::REFLECTION:
            ++reflection_num;
            texture_name = GetVariableName(reflection_str, reflection_num);
            break;
        }

        // Bind texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
        glUniform1i(glGetUniformLocation(shader.GetId(), texture_name.c_str()), i);
    }

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, amount);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

Model::Model(std::string path) {
    LoadModel(path);
}

void Model::Draw(helper::Shader& shader, const unsigned amount) const {
    for (const auto& mesh : meshes) {
        mesh.Draw(shader, amount);
    }
}

/**
 * \brief
 * \details
 * \param
 * \return  void
 *
 * \see     https://learnopengl.com/#!Model-Loading/Model
 */
void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;

    /**
     * First argument is path to read file, Second are post-processing options.
     *
     * aiProcess_GenNormals : actually creates normals for each vertex if the mo
     * del didn't contain normal vectors.
     * aiProcess_SplitLargeMeshes : splits large meshes into smaller sub-meshes
     * which is useful if your rendering has a maximum number of vertices allowe
     * d and can only process smaller meshes.
     * aiProcess_OptimizeMeshes : actually does the reverse by trying to join se
     * veral meshes into one larger mesh, reducing drawing calls for optimizatio
     * n.
     */
    const auto* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    else {
        std::cerr << "ASSIMP::SUCCESS" << std::endl;
        directory = path.substr(0, path.find_last_of('/'));
        ProcessNode(scene->mRootNode, scene);
    }
}

void Model::ProcessNode(aiNode* const node, const aiScene* const scene) {
    // Process all the node's meshes (if any)
    for (decltype(node->mNumMeshes) i = 0; i < node->mNumMeshes; ++i) {
        auto* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // Process all child of the node's children (if any)
    for (decltype(node->mNumChildren) i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* const mesh, const aiScene* const scene) {
    std::vector<Vertex>     vertices;
    std::vector<unsigned>   indices;
    std::vector<Texture>    textures;

    for (decltype(mesh->mNumVertices) i = 0; i < mesh->mNumVertices; ++i) {
        // Process vertex positions, normal and texture coordinates
        glm::vec3 position  { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        glm::vec3 normal    { mesh->mNormals[i].x   , mesh->mNormals[i].y, mesh->mNormals[i].z };
        glm::vec2 tex_coord { 0 ,0 };

        // If TextureCoord is present, refresh tex_coord with actual values;
        if (mesh->mTextureCoords[0])  {
            auto texture_coords = mesh->mTextureCoords[0];
            tex_coord = glm::vec2{ texture_coords[i].x, texture_coords[i].y };
        }

        vertices.push_back(Vertex{ position, normal, tex_coord });
    }

    // Process indices
    for (decltype(mesh->mNumFaces) i = 0; i < mesh->mNumFaces; ++i) {
        auto face = mesh->mFaces[i];
        for (decltype(face.mNumIndices) j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process material information
    if (mesh->mMaterialIndex >= 0) {
        auto* material = scene->mMaterials[mesh->mMaterialIndex];

        // Diffuse & Ambient textures
        std::vector<Texture> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                 TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        // Specular textures
        std::vector<Texture> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                  TextureType::SPECULAR);
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

        // Emission textures
        std::vector<Texture> emission_maps = LoadMaterialTextures(material, aiTextureType_EMISSIVE,
                                                                  TextureType::EMISSION);
        textures.insert(textures.end(), emission_maps.begin(), emission_maps.end());

        // Reflection textures
        std::vector<Texture> reflection_maps = LoadMaterialTextures(material, aiTextureType_AMBIENT,
                                                                    TextureType::REFLECTION);
        textures.insert(textures.end(), reflection_maps.begin(), reflection_maps.end());
    }

    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

unsigned TextureFromFile(const char* path, const std::string& directory);
std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* const mat,
                                                 aiTextureType type,
                                                 TextureType tex_type) {
    std::vector<Texture> textures;

    for (auto i = 0u; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::cerr << str.C_Str() << std::endl;

        bool skip = false;
        for (auto j = 0u; j < loaded_textures.size(); ++j) {
            if (std::strcmp(loaded_textures[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(loaded_textures[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            auto texture = Texture{ TextureFromFile(str.C_Str(), directory),
                tex_type,
                std::string(str.C_Str()) };

            textures.push_back(texture);
            loaded_textures.push_back(texture);
        }
    }

    return textures;
}


unsigned TextureFromFile(const char* path, const std::string& directory) {
    GLuint texture;
    glGenTextures(1, &texture);

    // Set the texture wrapping and filtering options, load image file to use as texture image.
    auto width = 0;
    auto height = 0;
    auto nr_channels = 0;

    auto filename = directory + "/" + std::string(path);

    auto data = stbi_load(filename.c_str(), &width, &height, &nr_channels, 0);
    if (data) {
        GLenum format;
        switch (nr_channels) {
        case 1: format = GL_RED;    break;
        case 3: format = GL_RGB;    break;
        case 4: format = GL_RGBA;   break;
        }

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cerr << "FAILED::LOAD::TEXTURE" << std::endl;
        throw std::runtime_error{ ("FAILED::LOAD::TEXTURE, " + filename).c_str() };
    }

    stbi_image_free(data);
    return texture;
}

}
