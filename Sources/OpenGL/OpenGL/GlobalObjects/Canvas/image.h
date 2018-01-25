#ifndef OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H
#define OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H

/**
 * @file GlobalObjects\Canvas\image.h
 * @brief Image object used in Canvas hierarchy.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <memory>
#include <type_traits>
#include "..\..\Headers\common.h"

namespace Canvas {
class Image : public Object {
public:
	explicit Image(std::string&& image_path);
	virtual ~Image() = default;

	/**
	 * @brief The method updates components of object.
	 */
	[[noreturn]] virtual void Update() override final {};

	/**
	 * @brief The method calls scene to one objects.
	 * @param[in] shader Shader to use.
	 */
	[[noreturn]] virtual void Draw(helper::ShaderNew& shader) override final;

	/**
	 * @brief This calls callee to draw or render something it has.
	 */
	[[noreturn]] virtual void Draw() override final;

	/**
	 * @brief This initiate object as a child of base object.
	 *
	 * This method create & initiate object as a child, make a leaf of object hierarchy tree.
	 * Created type instance is implicitly converted (up-cast) to Object class type.
	 *
	 * If you must initiate specific derived class with initialization parameters,
	 * you can type them _args parameter. This method gets variadic inputs initialized with
	 * appropriate c-tor.
	 *
	 * You have to <> parenthesis to input specific class type to create.
	 *
	 * @param[in] object Object instance to make.
	 * @param[in] tag Object Tag.
	 * @param[in] _args variadic args to be used c-tor initialize parameters inputs.
	 * @return Success/Failed flag. If the methods success to make child object, return true.
	 */
	template <class _Ty, class... _Types, class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool InitiateChild(const std::string&& tag, _Types&&... _args) {
		if (children.find(tag) != children.end()) return FAILED;

		children[tag] = std::make_shared<_Ty>(args...);
		return SUCCESS;
	}

	/**
	 * @brief Destroy child object has unique tag key.
	 * @param[in] tag Object tag.
	 * @return Success/Failed tag. If arbitary objects has been destroied, return ture.
	 */
	bool DestroyChild(std::string& tag);

	/**
	 * @brief Get children tag list.
	 */
	const std::vector<std::string> GetChildrenTags() const;

private:
	void InitiateShader();

private:
	helper::BindingObject quad;
	texture::Texture2D texture;
	std::shared_ptr<helper::ShaderNew> shader;

	std::unordered_map<std::string, std::shared_ptr<Object>> children;

	const float alpha{ 1.0f };
	const float width{ 128.f }, height{ 128.f };
};
}

#endif /** OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H */
