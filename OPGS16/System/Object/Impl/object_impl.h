#ifndef OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H
#define OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H

#include <unordered_map>
#include <glm\glm.hpp>

class ObjectImpl final {
public:
	inline const glm::vec3 GetLocalPosition() const { return m_local_position; }

	[[noreturn]] inline void SetLocalPosition(const glm::vec3& position) {
        m_final_position += (position - m_local_position);
        m_local_position = position;

        m_model_matrix_deprecated = true;
        m_translate_deprecated = true;
	}

	inline const glm::vec3 GetFinalPosition() const {
        return m_final_position;
    }

	inline void SetFinalPosition(const glm::vec3& final_position) {
		m_final_position = final_position;
        m_model_matrix_deprecated = true;
        m_translate_deprecated = true;
	}

	inline void UpdateFinalPosition(const glm::vec3& parent_position) {
		m_final_position = m_local_position + parent_position;
        m_model_matrix_deprecated = true;
        m_translate_deprecated = true;
	}

	inline const float GetRotationAngle() const {
        return m_rotation_angle;
    }

	[[noreturn]] inline void SetAngle(const float angle_value) {
		m_rotation_angle = angle_value;

        m_model_matrix_deprecated = true;
		m_rotation_deprecated = true;
	}

	inline const glm::vec3 GetRotationFactor() const {
        return m_rotation_factor;
    }

	[[noreturn]] inline void SetRotationFactor(const glm::vec3& factor) {
		m_rotation_factor = factor;

        m_model_matrix_deprecated = true;
		m_rotation_deprecated = true;
	}

	inline const float GetScaleValue() const {
        return m_scale_value;
    }

	[[noreturn]] inline void SetScaleValue(const float scale_value) {
		m_scale_value = scale_value;

        m_model_matrix_deprecated = true;
		m_scale_deprecated = true;
	}

	inline const glm::vec3 GetScaleFactor() const {
        return m_scale_factor;
    }

	[[noreturn]] void SetScaleFactor(const glm::vec3& scale_factor) {
		m_scale_factor = scale_factor;

        m_model_matrix_deprecated = true;
		m_scale_deprecated = true;
	}

	const glm::mat4 GetModelMatrix();

	/**
	 * @brief Set active option of object.
	 * If m_active is false, this object cannot update until m_active return to true.
	 * @param[in] value Active option value.
	 */
	[[noreturn]] void SetActive(const bool value) { m_active = value; }

	/** Get active value. */
	inline bool GetActiveValue() const { return m_active; }

private:
    /** (x, y, z) local position */
    glm::vec3   m_local_position{};
	/** (x, y, z) final position in hierarchy */
	glm::vec3	m_final_position{};

    /** Rotation angle : Positive value is CW, Negative value is CCW */
    float       m_rotation_angle{};
    /** Rotation factor is (x, y, z) factor did not exceed [-1, 1] range. */
    glm::vec3   m_rotation_factor{ 1.0f };

    /** Scale value's default value is 1.0f */
    float       m_scale_value{ 1.0f };
    /** Scale factor is (x, y, z) factor default is (1.0f, 1.0f, 1.0f) */
    glm::vec3   m_scale_factor{ 1.0f };

    glm::mat4   m_model;
    glm::mat4   m_translate;
    glm::mat4   m_rotate;
    glm::mat4   m_scale;

    //bool m_is_changed{ true };
	bool m_active{ true };

    mutable bool m_model_matrix_deprecated{ true };
    mutable bool m_translate_deprecated{ true };
    mutable bool m_rotation_deprecated{ true };
    mutable bool m_scale_deprecated{ true };

private:
	void RefreshTranslateMatrix();	/** Refresh Translation matrix */
	void RefreshRotateMatrix();		/** Refresh Rotation matrix */
	void RefreshScaleMatrix();		/** Refresh Scaling matrix */
};

#endif /** OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H */