#ifndef OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H
#define OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H

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
 * @file System\Object\Impl\object_impl.h
 * @brief Pointer to implementation file of Object.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-19 Refactoring, Inlining, and Removed not-used variables. add class description.
 * 2018-02-19 Add GetParentPosition() method, returns m_parent_to_position.
 * 2018-02-23 Add succeeding flag of translation, rotation, scaling from parent.
 * 2018-03-05 Add rendering layer member functions.
 */

#include <string>       /*! std::string */
#include <glm/glm.hpp>  /*! glm::vec_x */

/*!
 * @class ObjectImpl
 * @brief Pointer to implementation of class Object.
 */
class ObjectImpl final {
public:
    /*! Position */

	inline const glm::vec3& GetLocalPosition() const {
        return m_local_position;
    }

	inline const glm::vec3& GetWorldPosition() const {
        return m_world_position;
    }

    inline const glm::vec3& GetParentPosition() const {
        return m_parent_to_position;
    }

    inline const glm::vec3& GetFinalPosition() const {
        if (m_final_pos_deprecated) {
            m_final_position = m_parent_from_position + m_world_position + m_local_position;
            m_final_pos_deprecated = false;
        }

        return m_final_position;
    }

	inline void SetLocalPosition(const glm::vec3& position) {
        m_local_position = position;
        m_model_matrix_deprecated = true;
        m_final_pos_deprecated = true;
	}

    inline void SetWorldPosition(const glm::vec3& position) {
        m_world_position        = position;
        m_parent_to_position    = m_parent_from_position + m_world_position;

        m_model_matrix_deprecated = true;
        m_final_pos_deprecated = true;
    }

	inline void SetParentPosition(const glm::vec3& parent_position) {
        m_parent_from_position  = parent_position;
        m_parent_to_position    = parent_position + m_world_position;

        m_model_matrix_deprecated = true;
        m_final_pos_deprecated = true;
	}

    /*! Angle */

	inline const float GetRotationLocalAngle() const noexcept {
        return m_rotation_local_angle;
    }

	inline const glm::vec3& GetRotationLocalFactor() const noexcept {
        return m_rotation_local_factor;
    }

    inline const float GetRotationParentAngle() const noexcept {

    }

    inline const glm::vec3& GetRotationParentFactor() const noexcept {

    }

    inline const glm::vec3& GetRotationFinalAngle() const noexcept {

    }

    inline const glm::vec3& GetRotationFinalFactor() const noexcept {

    }

	inline void SetRotationLocalAngle(const float angle_value) noexcept {
		m_rotation_local_angle = angle_value;

        m_model_matrix_deprecated = true;
		m_rotation_deprecated = true;
	}

	inline void SetRotationLocalFactor(const glm::vec3& factor) noexcept {
		m_rotation_local_factor = factor;

        m_model_matrix_deprecated = true;
		m_rotation_deprecated = true;
	}

    inline void SetRotationParentAngle(const float angle_value) noexcept {

    }

    inline void SetRotationParentFactor(const glm::vec3& factor) noexcept {

    }

    /*! Scale */

	inline const float GetScaleLocalValue() const noexcept {
        return m_scale_local_value;
    }

	inline const glm::vec3& GetScaleLocalFactor() const noexcept {
        return m_scale_local_factor;
    }

    inline const float GetScaleParentValue() const noexcept {

    }

    inline const glm::vec3& GetScaleParentFactor() const noexcept {

    }

    inline const float GetScaleFinalValue() const noexcept {

    }

    inline const glm::vec3& GetScaleFinalFactor() const noexcept {

    }

	inline void SetScaleLocalValue(const float scale_value) noexcept {
		m_scale_local_value = scale_value;

        m_model_matrix_deprecated = true;
		m_scale_deprecated = true;
	}

	inline void SetScaleLocalFactor(const glm::vec3& scale_factor) noexcept {
		m_scale_local_factor = scale_factor;

        m_model_matrix_deprecated = true;
		m_scale_deprecated = true;
	}

    inline void SetScaleParentValue(const float scale_value) noexcept {
        m_scale_parent_value = scale_value;
    }

    inline void SetScaleParentFactor(const glm::vec3& scale_factor) noexcept {
        m_scale_parent_factor = scale_factor;
    }

    /*! Matrix */

	const glm::mat4& GetModelMatrix() const;

    /*! Flag inline methods */

    inline void SetSucceedingPositionFlag(bool value) {
        m_position_succeedable = value;
    }

    inline void SetSucceedingRotationFlag(bool value) {
        m_rotation_succeedable = value;
    }

    inline void SetSucceedingScalingFlag(bool value) {
        m_scaling_succeedable = value;
    }

    inline bool GetSucceedingPositionFlag() {
        return m_position_succeedable;
    }

    inline bool GetSucceedingRotationFlag() {
        return m_rotation_succeedable;
    }

    inline bool GetSucceedingScalingFlag() {
        return m_scaling_succeedable;
    }

	/**
	 * @brief Set active option of object.
	 * If m_active is false, this object cannot update until m_active return to true.
	 * @param[in] value Active option value.
	 */
	inline void SetActive(const bool value) {
        m_active = value;
    }

    /*! Get active value. */
	inline bool GetActive() const {
        return m_active;
    }

    /*!
     * @brief Set tag with tag name. This method will check whether or not exist matched tag name
     * in SettingManager. If not exist, do nothing and chagne error flag.
     * @param[in] tag_name Tag name
     */
    void SetTag(const std::string& tag_name);

    /*! Overloading version of SetTag(tag_name) */
    void SetTag(const size_t tag_index);

    /*! Return tag index value of this object. */
    inline size_t GetTagIndexOf() const noexcept {
        return m_tag_index;
    }

    /*!
     * @brief Get Tag Name of this object. (different with name of object)
     * This methods return tag name by referencing SettingManager in body.
     * @return Tag name string.
     */
    std::string GetTagNameOf() const;

    void SetRenderLayer(const std::string& layer_name);

    void SetRenderLayer(const size_t layer_index);

    inline size_t RenderLayerIndexOf() const noexcept {
        return m_render_layer_index;
    }

    std::string RenderLayerNameOf() const;

private:
    glm::vec3   m_local_position{};             /*! (x, y, z) local position. */
    glm::vec3   m_world_position{};             /*! (x, y, z) world position. */
    glm::vec3   m_parent_from_position{};       /*! (x, y, z) final position of parent. */
    mutable glm::vec3 m_parent_to_position{};   /*! (x, y, z) parent position to bring child. */
	mutable glm::vec3 m_final_position{};       /*! (x, y, z) final position in hierarchy. */

    float       m_rotation_local_angle{};       /*! Rotation angle. Positive CW, Negative CCW */
    glm::vec3   m_rotation_local_factor{ 1.f }; /*! Rotation factor is (x, y, z) factor */
    float       m_rotation_parent_from_angle{}; /*! Rotation angle value from parent */
    glm::vec3   m_rotation_parent_from_factor{};/*! Rotation factor from parent */
    mutable float m_rotation_final_angle{};     /*! Final rotation angle */
    mutable glm::vec3 m_rotation_final_factor{ 1.f };   /*! Final rotation factor */

    mutable glm::mat4 m_rotate_matrix;          /*! Rotate matrix */

    float       m_scale_local_value{ 1.f };     /*! Scale value's default value is 1.0f */
    glm::vec3   m_scale_local_factor{ 1.f };    /*! Scale local factor, default is (1, 1, 1) */
    float       m_scale_parent_value{ 1.f };    /*! Scale value from parent */
    glm::vec3   m_scale_parent_factor{ 1.f };   /*! Scale factor from parent */
    mutable glm::vec3 m_scale_final_vector{};   /*! (x, y, z) scale vector to apply to matrix */

    mutable glm::mat4   m_model{};          /*! Model matrix */

	bool m_active{ true };                  /*! Object update activation variable. */
    bool m_position_succeedable{ true };    /*! Flag for succeeding parent position */
    bool m_rotation_succeedable{ true };    /*! Flag for succeeding parent rotation information */
    bool m_scaling_succeedable{ true };     /*! Flag for succeeding parent scaling information */

    mutable bool m_model_matrix_deprecated{ true }; /*! The flag model needs to be updated. */
    mutable bool m_final_pos_deprecated{ true };    /*! The flag final pos needs to be updated. */
    mutable bool m_rotation_deprecated{ true };     /*! The flag rotation needs to be updated. */
    mutable bool m_scale_deprecated{ true };        /*! The flag scale vec needs to be updated. */

    size_t m_tag_index{ 0 };                /*! Tag index */
    size_t m_render_layer_index{ 0 };       /*! Rendering layer index */

private:
	void RefreshFinalPosition() const;	/** Refresh Translation matrix */
	void RefreshRotateMatrix() const;	/** Refresh Rotation matrix */
	void RefreshScaleVector() const;	/** Refresh Scaling matrix */
};

#endif /** OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H */