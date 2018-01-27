#include "object.h"

Object::Object() : camera{ camera::GetCamera() } {}

Object::~Object() {}

glm::vec3 Object::GetLocalPosition() {
    return position;
}

void Object::SetLocalPosition(glm::vec3 position) {
    this->position = position;

    /** Renew translate matrix */
    RefreshTranslateMatrix();
    is_changed = true;
}

glm::vec3 Object::GetFinalPosition() {
	return final_position;
}

void Object::RefreshFinalPosition(const glm::vec3& parent_position) {
	final_position = position + parent_position;
}

/**
 * @brief The method gets rotation values
 * @return Object's rotation angle value.
 */
float Object::GetRotationAngle() {
    return rotation_angle;
}

/**
 * @brief The method sets rotation angle values.
 *
 * When input value is positive and factor is [0, 1], axis rotates clockwise.
 * input value is negative and factor is [0, 1], axis rotates counter-clockwise.
 *
 * @param[in] angle_value Angle value to set on.
 */
void Object::SetAngle(const float angle_value) {
    this->rotation_angle = angle_value;

    /** Renew rotation matrix */
    RefreshRotateMatrix();
    is_changed = true;
}

/**
 * @brief The method gets (x, y, z) glm::vec3 rotation axis factor.
 * @return Object's rotation vector which has (x, y, z) rotation axis factor.
 */
glm::vec3 Object::GetRotationFactor() {
    return rotation_factor;
}

/**
 * @brief The method sets rotation factor have values which ranges [-1, 1].
 * @param[in] factor
 */
void Object::SetRotationFactor(glm::vec3 factor) {
    rotation_factor = factor;

    /** Renew rotation matrix */
    RefreshRotateMatrix();
    is_changed = true;
}

/**
 * @brief The method gets scaling values
 * @return Object's scaling value.
 */
float Object::GetScaleValue() {
    return scale_value;
}

/**
 * @brief The method sets scaling angle values.
 * @param[in] scale_value Scaling value to set on.
 */
void Object::SetScaleValue(const float scale_value) {
    this->scale_value = scale_value;

    /** Renew scale matrix */
    RefreshScaleMatrix();
    is_changed = true;
}

/**
 * @brief The method gets (x, y, z) glm::vec3 scaling axis factor.
 * @return Object's scaling vector which has (x, y, z) axis factors.
 */
glm::vec3 Object::GetScaleFactor() {
    return scale_factor;
}

/**
 * @brief The method sets scaling vector have (x, y, z) scaling factors.
 * @param[in] factor Scaling factor
 */
void Object::SetScaleFactor(glm::vec3 factor) {
    scale_factor = factor;

    /** Renew scale matrix */
    RefreshScaleMatrix();
    is_changed = true;
}

/**
 * @brief The method returns Model matrix, M = TRS
 *
 * If any value which related to Translate, Rotation, Scaling has been changed,
 * model matrix would be calculated newly and returned.
 *
 * @return Model matrix (M = TRS)
 */
glm::mat4 Object::GetModelMatrix() {
    if (is_changed) {
        model = glm::mat4{} * translate * rotate * scale;
        is_changed = false;
    }

    return model;
}

 bool Object::DestroyChild(const std::string & tag) {
	 decltype(children.end()) iterator = children.find(tag);
	 if (iterator == children.end())
		 return 0;

	 children.erase(tag);
	 return 1;
 }

 const std::vector<std::string> Object::GetChildrenTags() const {
	 std::vector<std::string> list(children.size());
	 for (const auto& object_pair : children) {
		 list.push_back(object_pair.first);
	 }
	 return list;
 }

 std::unordered_map<std::string, std::shared_ptr<Object>>& Object::GetChildren() {
	 return children;
 }

 std::shared_ptr<Object> Object::GetChild(const std::string& tag) {
	 if (children.find(tag) == children.end()) return nullptr;
	 return children.at(tag);
 }

/**
 * @brief Refresh Translation matrix
 */
void Object::RefreshTranslateMatrix() {
    translate = glm::translate(glm::mat4{}, position + final_position);
}

/**
 * @brief Refresh Rotation matrix
 */
void Object::RefreshRotateMatrix() {
    rotate = glm::rotate(glm::mat4{}, glm::radians(rotation_angle), rotation_factor);
}

/**
 * @brief Refresh Scaling matrix
 */
void Object::RefreshScaleMatrix() {
    scale = glm::scale(glm::mat4{}, scale_value * scale_factor);
}