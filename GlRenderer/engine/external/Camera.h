#pragma once

#include <memory>
#include <functional>
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

class Camera {
private:
	struct M {
		std::shared_ptr<glm::mat4> proj;
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;
	} m;
	
	explicit Camera(M m) : m(std::move(m)) {}

public:
	static Camera create();

	void update_aspect_ratio(float aspect_ratio);
	void update(ImVec2& viewportSize);

	void invoke() const;

	inline std::shared_ptr<glm::mat4> get_proj() const { return m.proj; }

	struct WithResultOf {
		std::function<Camera()> func;
	};

	Camera(WithResultOf&& res) noexcept;
};