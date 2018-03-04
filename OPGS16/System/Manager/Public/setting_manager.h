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
 * @class SettingManager
 * @brief Setting Manager.
 *
 * @log
 * 2018-02-26 Removed redundant move special functions.
 * 2018-03-04 Refactoring.
 */
class SettingManager final {
public:
    static SettingManager& Instance() {
        static SettingManager instance{};
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

private:
    std::vector<std::string> m_tag_list;    /*! Tag list used in Application. */

private:
    /*!
     * @brief Initialize tag list using file stream.
     * This sequence proceed loading synchronously.
     */
    void InitializeTagList();

    /*!
     * @brief Initailize layer list using file stream.
     * NOT IMPLEMENTED YET.
     */
    void InitializeLayerList();

private:
    SettingManager();

public:
    SettingManager(const SettingManager&) = delete;
    SettingManager& operator=(const SettingManager&) = delete;
};

} /*! opgs16::manager */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_MANAGER_SETTING_MANAGER_H
