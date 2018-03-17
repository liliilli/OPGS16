#ifndef OPGS16_SYSTEM_MANAGER_SETTING_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_SETTING_MANAGER_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System/Manager/Public/setting_manager.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-26 Removed redundant move special functions.
 * 2018-03-04 Refactoring.
 */

#include <string>   /*! std::string */
#include <vector>   /*! std::vector */

namespace opgs16 {
namespace manager {

/*!
 * @class MSettingManager
 * @brief Setting Manager.
 *
 * @log
 * 2018-02-26 Removed redundant move special functions.
 * 2018-03-04 Refactoring.
 */
class MSettingManager final {
public:
    static MSettingManager& Instance() {
        static MSettingManager instance{};
        return instance;
    }

    /*!
     * @brief Return tag name of specified index (id).
     * If input id exceeds the size of tag list, return "ERROR::TAG::OOB"
     * @param[in] id index number to find in tag list.
     * @return Tag name of index.
     */
    std::string GetTagName(const size_t id);

    /*!
     * @brief Return tag name list.
     * @return Tag name list with vector.
     */
    const std::vector<std::string>& GetTagNameList() const;

    /*!
     * @brief Return rendering layer name of indicated index.
     * @param[in] id index number to find in rendering layer list.
     * @return Rendering name of index.
     */
    inline std::string RenderingLayerName(const size_t id) const noexcept {
        if (id >= m_render_layer_list.size()) return "";
        else return m_render_layer_list[id];
    }

    /*!
     * @brief Return rendering layer name list.
     * @return Rendering layer name list.
     */
    const std::vector<std::string>& RenderingLayerNameList() const noexcept {
        return m_render_layer_list;
    }

    /*! Return rendering layer list size. */
    inline size_t RenderingLayerListSize() const noexcept {
        return m_render_layer_list.size();
    }

    /*! */
    inline std::string CollisionLayerName(const size_t id) const noexcept {
        if (id >= m_collision_layer_list.size()) return "";
        else return m_collision_layer_list[id];
    }

    /*! Return collision layer list */
    const std::vector<std::string>& CollisionLayerList() const noexcept {
        return m_collision_layer_list;
    }

    /*! Get collision check flag of collision layers */
    bool CollisionCheck(unsigned src, unsigned dst) const noexcept {
        const auto size = m_collision_check_list.size();
        if (src > dst)  return m_collision_check_list[dst][size - src - 1];
        else            return m_collision_check_list[src][size - dst - 1];
    }

private:
    std::vector<std::string> m_tag_list;                /*! Tag list used in Application. */
    std::vector<std::string> m_render_layer_list;       /*! Render layer list */
    std::vector<std::string> m_collision_layer_list;    /*! Collision Layer list */
    std::vector<std::vector<bool>> m_collision_check_list;

    MSettingManager();

public:
    MSettingManager(const MSettingManager&) = delete;
    MSettingManager& operator=(const MSettingManager&) = delete;
};

} /*! opgs16::manager */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_MANAGER_SETTING_MANAGER_H
