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
#include <spdlog/spdlog.h>
	
class UniformObject {
private:
	struct MVP {
		std::shared_ptr<glm::mat4> model; // controlled by model
		std::shared_ptr<glm::mat4> view; // controlled by camera
		std::shared_ptr<glm::mat4> proj; // controlled by camera
	};

	struct M {
		uint32_t UBO;
		uint32_t program;
		uint32_t binding;
		MVP mvp;
	} m;

	explicit UniformObject(M m) : m(std::move(m)) {}

public:
	struct Parameters {
		std::shared_ptr<glm::mat4> model;
		std::unique_ptr<Camera>& camera;
		const uint32_t& program;
		const uint32_t& binding;
	};

	static UniformObject create(Parameters&& parameters);

	void invoke();

	void update();

	void revoke();

	struct WithResultOf {
		std::function<UniformObject()> func;
	};

	UniformObject(WithResultOf&& res) noexcept;
};