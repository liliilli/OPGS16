#ifndef OPGS16_SYSTEM_MANAGER_SETTING_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_SETTING_MANAGER_H

/*!
 * @file SettingManager
 * @author Jongmin Yun
 * @date 2018-02-16
 */

#include <string>   /*! std::string */
#include <vector>   /*! std::vector */

/*!
 * @class SettingManager
 * @brief Setting Manager.
 */
class SettingManager final {
public:
    static SettingManager& GetInstance() {
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
    static constexpr char s_tag_file[] = R"(_Setting/tag.meta)";
    static constexpr char s_layer_file[] = R"(_Setting/layer.meta)";

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
    SettingManager(const SettingManager&) = delete;
    SettingManager(SettingManager&&) = delete;
    SettingManager& operator=(const SettingManager&) = delete;
    SettingManager& operator=(SettingManager&&) = delete;
};

#endif // !OPGS16_SYSTEM_MANAGER_SETTING_MANAGER_H
