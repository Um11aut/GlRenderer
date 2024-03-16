#pragma once

#include <glad/glad.h>
#include <string>
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "external/Camera.h"
	
class UniformObject {
private:
	struct MVP {
		glm::mat4 model;
		glm::mat4 view;
		std::shared_ptr<glm::mat4> proj;
	};

	struct M {
		uint32_t UBO;
		uint32_t program;
		MVP mvp;
	} m;

	explicit UniformObject(M m) : m(std::move(m)) {}

public:
	struct Parameters {
		std::unique_ptr<Camera>& camera;
		const uint32_t& program;
		const uint32_t& binding;
	};

	static UniformObject create(Parameters&& parameters);

	void invoke();
	void destroy() const;

	struct WithResultOf {
		std::function<UniformObject()> func;
	};

	UniformObject(WithResultOf&& res) noexcept;
};