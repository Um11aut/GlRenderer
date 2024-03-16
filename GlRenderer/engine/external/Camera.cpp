#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <spdlog/spdlog.h>

Camera Camera::create()
{
	float fov = 45.f;
	float aspectRatio = (float)800 / (float)600;
	float nearPlane = .1f;
	float farPlane = 100.f;
	glm::mat4 proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	

	return Camera(M{
		std::make_shared<glm::mat4>(proj),
		fov,
		aspectRatio,
		nearPlane,
		farPlane
		});
}

void Camera::update_aspect_ratio(float aspect_ratio)
{
	m.aspectRatio = aspect_ratio;
}

void Camera::update(ImVec2& viewport)
{
	spdlog::info("Changed Scene viewport size!: {}, {}", viewport.x, viewport.y);
	glm::mat4 proj = glm::perspective(glm::radians(45.f), viewport.x / viewport.y, m.nearPlane, m.farPlane);
	*m.proj = proj;
}

Camera::Camera(WithResultOf&& res) noexcept
{
	auto other = res.func();

	m = std::move(other.m);
}
