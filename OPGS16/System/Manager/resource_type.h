#pragma once

#include <string>       /*! std::string */
#include <glm\glm.hpp>  /*! glm vector types */

/*!
 * @namespace resource
 * @brief Container namespace for all resource type used in ResourceManager.

 * @author Jongmin Yun
 * @date 2018-02-28
 * @log
 * 2018-02-28 Create namespace.
 */
namespace resource {

/*! Resource type */
enum class ScopeType {
    GLOBAL,     /*! G */
    SCENE_LOCAL /*! L */
};

ScopeType GetScopeType(char type);

/*! Information structure of Texture2D */
struct Texture2D {
    struct IndexSize { unsigned x_sep, y_sep; };

    ScopeType       m_type;
    std::string     m_path;
    IndexSize       m_nm_size;
};

}
