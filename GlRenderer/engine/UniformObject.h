#pragma once

#include <glad/glad.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
	
class UniformObject {
private:
	struct MVP {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	struct M {
		uint32_t UBO;
		uint32_t program;
		MVP mvp;
	} m;

	explicit UniformObject(M m) : m(std::move(m)) {}

public:
	static UniformObject create(const uint32_t& program, const uint32_t& binding);

	void invoke() const;
	void destroy() const;

	struct WithResultOf {
		std::function<UniformObject()> func;
	};

	UniformObject(WithResultOf&& res) noexcept;
};