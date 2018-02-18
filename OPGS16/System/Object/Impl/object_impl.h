#ifndef OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H
#define OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H

#include <unordered_map>
#include <glm\glm.hpp>

class ObjectImpl final {
public:
    /*! Position */

	inline const glm::vec3 GetLocalPosition() const {
        return m_local_position;
    }

	 inline void SetLocalPosition(const glm::vec3& position) {
        m_local_position = position;
        m_model_matrix_deprecated = true;
        m_translate_deprecated = true;
        m_final_pos_deprecated = true;
	}

	inline const glm::vec3 GetWorldPosition() const {
        return m_world_position;
    }

     inline void SetWorldPosition(const glm::vec3& position);

	 inline void SetParentPosition(const glm::vec3& parent_position) {
        m_parent_position = parent_position;
        m_model_matrix_deprecated = true;
        m_translate_deprecated = true;
        m_final_pos_deprecated = true;
	}

    inline const glm::vec3 GetFinalPosition() const {
        if (m_final_pos_deprecated) {
            m_final_position = m_parent_position + m_world_position + m_local_position;
            m_final_pos_deprecated = false;
        }

        return m_final_position;
    }

    /*! Angle */

	inline const float GetAngle() const {
        return m_rotation_angle;
    }

	 inline void SetAngle(const float angle_value) {
		m_rotation_angle = angle_value;

        m_model_matrix_deprecated = true;
		m_rotation_deprecated = true;
	}

	inline const glm::vec3 GetRotationFactor() const {
        return m_rotation_factor;
    }

	 inline void SetRotationFactor(const glm::vec3& factor) {
		m_rotation_factor = factor;

        m_model_matrix_deprecated = true;
		m_rotation_deprecated = true;
	}

    /*! Scale */

	inline const float GetScaleValue() const {
        return m_scale_value;
    }

	 inline void SetScaleValue(const float scale_value) {
		m_scale_value = scale_value;

        m_model_matrix_deprecated = true;
		m_scale_deprecated = true;
	}

	inline const glm::vec3 GetScaleFactor() const {
        return m_scale_factor;
    }

	 void SetScaleFactor(const glm::vec3& scale_factor) {
		m_scale_factor = scale_factor;

        m_model_matrix_deprecated = true;
		m_scale_deprecated = true;
	}

    /*! Matrix */

	const glm::mat4 GetModelMatrix();

	/**
	 * @brief Set active option of object.
	 * If m_active is false, this object cannot update until m_active return to true.
	 * @param[in] value Active option value.
	 */
	 void SetActive(const bool value) { m_active = value; }

	inline bool GetActive() const { return m_active; } /** Get active value. */

    /*!
     * @brief Set tag with tag name. This method will check whether or not exist matched tag name
     * in SettingManager. If not exist, do nothing and chagne error flag.
     * @param[in] tag_name Tag name
     */
     void SetTag(const std::string tag_name);

    /*! Overloading version of SetTag(tag_name) */
     void SetTag(const size_t tag_index);

    /*!
     * @brief Get tag index of this object.
     * @return Tag index value.
     */
    inline size_t GetTagIndexOf() const;

    /*!
     * @brief Get Tag Name of this object. (different with name of object)
     * This methods return tag name by referencing SettingManager in body.
     * @return Tag name string.
     */
    std::string GetTagNameOf() const;

private:
    glm::vec3   m_local_position{};     /*! (x, y, z) local position. */
    glm::vec3   m_world_position{};     /*! (x, y, z) world position. */
    glm::vec3   m_parent_position{};    /*! (x, y, z) final position of parent. */

	mutable glm::vec3 m_final_position{};     /*! (x, y, z) final position in hierarchy. */

    float       m_rotation_angle{};         /*! Rotation angle. Positive is CW, Negative is CCW */
    glm::vec3   m_rotation_factor{ 1.0f };  /*! Rotation factor is (x, y, z) factor */

    float       m_scale_value{ 1.0f };  /*! Scale value's default value is 1.0f */
    glm::vec3   m_scale_factor{ 1.0f }; /*!  * Scale factor is (x, y, z) default is (1, 1, 1) */

    glm::mat4   m_model;
    glm::mat4   m_translate;
    glm::mat4   m_rotate;
    glm::mat4   m_scale;

	bool m_active{ true };

    mutable bool m_model_matrix_deprecated{ true };
    mutable bool m_translate_deprecated{ true };
    mutable bool m_final_pos_deprecated{ true };

    mutable bool m_rotation_deprecated{ true };
    mutable bool m_scale_deprecated{ true };

    size_t m_tag_index{ 0 };

private:
	void RefreshTranslateMatrix();	/** Refresh Translation matrix */
	void RefreshRotateMatrix();		/** Refresh Rotation matrix */
	void RefreshScaleMatrix();		/** Refresh Scaling matrix */
};


inline size_t ObjectImpl::GetTagIndexOf() const {
    return m_tag_index;
}

inline void ObjectImpl::SetWorldPosition(const glm::vec3& position) {
    m_world_position = position;
    m_model_matrix_deprecated = true;
    m_translate_deprecated = true;
    m_final_pos_deprecated = true;
}

#endif /** OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H */