#include "object.h"
#include "impl\object_impl.h"           /*! ObjectImpl */
#include "..\Debugs\hierarchy_tree.h"   /*! ObjectTree */

Object::Object() {
	std::unique_ptr<ObjectImpl, ObjectImplDeleter> instance{ new ObjectImpl() };
	m_data = std::move(instance);
}

void Object::Update() {
    for (auto& component : m_components) {
        component->Update();
    }

	for (auto& child : m_children) {
		child.second->UpdateFinalPosition(GetFinalPosition());
		child.second->Update();
	}
}

const glm::vec3 Object::GetLocalPosition() const {
    return m_data->GetLocalPosition();
}

void Object::SetLocalPosition(glm::vec3 position) {
	m_data->SetLocalPosition(position);
}

const glm::vec3 Object::GetFinalPosition() const {
    return m_data->GetFinalPosition();
}

void Object::SetFinalPosition(const glm::vec3& final_position) {
	m_data->SetFinalPosition(final_position);
}

void Object::UpdateFinalPosition(const glm::vec3& parent_position) {
	m_data->UpdateFinalPosition(parent_position);

	for (auto& child : m_children)
		child.second->UpdateFinalPosition(m_data->GetFinalPosition());
}

/**
 * @brief The method gets rotation values
 * @return Object's rotation angle value.
 */
const float Object::GetRotationAngle() const { return m_data->GetRotationAngle(); }

/**
 * @brief The method sets rotation angle values.
 *
 * When input value is positive and factor is [0, 1], axis rotates clockwise.
 * input value is negative and factor is [0, 1], axis rotates counter-clockwise.
 *
 * @param[in] angle_value Angle value to set on.
 */
void Object::SetAngle(const float angle_value) {
	m_data->SetAngle(angle_value);
}

/**
 * @brief The method gets (x, y, z) glm::vec3 rotation axis factor.
 * @return Object's rotation vector which has (x, y, z) rotation axis factor.
 */
const glm::vec3 Object::GetRotationFactor() const { return m_data->GetRotationFactor(); }

/**
 * @brief The method sets rotation factor have values which ranges [-1, 1].
 * @param[in] factor
 */
void Object::SetRotationFactor(const glm::vec3& factor) {
	m_data->SetRotationFactor(factor);
}

/**
 * @brief The method gets scaling values
 * @return Object's scaling value.
 */
const float Object::GetScaleValue() const { return m_data->GetScaleValue(); }

/**
 * @brief The method sets scaling angle values.
 * @param[in] scale_value Scaling value to set on.
 */
void Object::SetScaleValue(const float scale_value) {
	m_data->SetScaleValue(scale_value);
}

/**
 * @brief The method gets (x, y, z) glm::vec3 scaling axis factor.
 * @return Object's scaling vector which has (x, y, z) axis factors.
 */
const glm::vec3 Object::GetScaleFactor() const { return m_data->GetScaleFactor(); }

/**
 * @brief The method sets scaling vector have (x, y, z) scaling factors.
 * @param[in] factor Scaling factor
 */
void Object::SetScaleFactor(const glm::vec3& factor) {
	m_data->SetScaleFactor(factor);
}

/**
 * @brief The method returns Model matrix, M = TRS
 * If any value which related to Translate, Rotation, Scaling has been changed,
 * model matrix would be calculated newly and returned.
 *
 * @return Model matrix (M = TRS)
 */
const glm::mat4 Object::GetModelMatrix() const { return m_data->GetModelMatrix(); }

 bool Object::DestroyChild(const std::string & tag) {
	 decltype(m_children.end()) iterator = m_children.find(tag);
	 if (iterator == m_children.end())
		 return 0;

	 m_children.erase(tag);
	 return 1;
 }

 const std::vector<std::string> Object::GetChildrenNameList() const {
	 std::vector<std::string> list(m_children.size());
	 for (const auto& object_pair : m_children) {
		 list.push_back(object_pair.first);
	 }

	 return list;
 }

 Object::object_map& Object::GetChildren() {
	 return m_children;
 }

 Object::object_raw Object::GetChild(const std::string& tag) {
	 if (m_children.find(tag) == m_children.end()) return nullptr;
	 return m_children.at(tag).get();
 }

bool Object::GetActiveValue() { return m_data->GetActiveValue(); }

void Object::SetActive(const bool value) {
	m_data->SetActive(value);
}

void Object::GetObjectTree(ObjectTree* const tree) {
	for (const auto& object : m_children) {
		ObjectTree child; child.name = object.first;
		tree->children.push_back(std::move(child));
		object.second->GetObjectTree(&*tree->children.rbegin());
	}
}

void Object::SetTag(const std::string tag_name) {
    m_data->SetTag(tag_name);
}

void Object::SetTag(const size_t tag_index) {
    m_data->SetTag(tag_index);
}

size_t Object::GetTagIndexOf() const {
    return m_data->GetTagIndexOf();
}

std::string Object::GetTagNameOf() const {
    return m_data->GetTagNameOf();
}


void ObjectImplDeleter::operator()(ObjectImpl* p) {
	delete p;
}
