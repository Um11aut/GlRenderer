#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <spdlog/spdlog.h>
#include <glm/ext/matrix_transform.hpp>

Camera Camera::create()
{
	float fov = 45.f;
	float aspectRatio = (float)800 / (float)600;
	float nearPlane = .1f;
	float farPlane = 100.f;
	glm::mat4 proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);


	glm::vec3 cameraPosition = glm::vec3(4, 3, 3);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(
		cameraPosition,
		cameraFront,
		cameraUp
	);
	
	ProjectionControls proj_controls{
		fov,
		aspectRatio,
		nearPlane,
		farPlane
	};

	ViewControls view_controls{
		cameraPosition,
		cameraFront,
		cameraUp
	};

	CameraControls cam_controls{
		.movementSpeed = 0.5f,
		.viewSensitivity = 0.1f,
		.yaw = -90.f,
		.pitch = 0.f,
	};

	return Camera(M{
		std::make_shared<glm::mat4>(proj),
		std::make_shared<glm::mat4>(view),
		std::make_shared<ProjectionControls>(proj_controls),
		std::make_shared<ViewControls>(view_controls),
		std::make_shared<CameraControls>(cam_controls),
		});
}

void Camera::update_projection(ImVec2& viewport)
{
	glm::mat4 proj = glm::perspective(glm::radians(m.proj_controls->fov), viewport.x / viewport.y, m.proj_controls->nearPlane, m.proj_controls->farPlane);
	*m.proj = proj;
}

void Camera::update_view()
{
	glm::mat4 view = glm::lookAt(
		m.view_controls->cameraPosition,
		m.view_controls->cameraPosition + m.view_controls->cameraFront,
		m.view_controls->cameraUp
	);
	*m.view = view;
}

void Camera::process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		auto delta_time = ImGui::GetIO().DeltaTime * 100.f;
		m.view_controls->cameraPosition += m.camera_controls->movementSpeed * m.view_controls->cameraFront * delta_time;
		update_view();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		auto delta_time = ImGui::GetIO().DeltaTime * 100.f;
		m.view_controls->cameraPosition -= m.camera_controls->movementSpeed * m.view_controls->cameraFront * delta_time;
		update_view();
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		auto delta_time = ImGui::GetIO().DeltaTime * 100.f;
		m.view_controls->cameraPosition -= glm::normalize(glm::cross(m.view_controls->cameraFront, m.view_controls->cameraUp)) * m.camera_controls->movementSpeed * delta_time;
		update_view();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		auto delta_time = ImGui::GetIO().DeltaTime * 100.f;
		m.view_controls->cameraPosition += glm::normalize(glm::cross(m.view_controls->cameraFront, m.view_controls->cameraUp)) * m.camera_controls->movementSpeed * delta_time;
		update_view();
	}
}

void Camera::rotate_camera(double& x_offset, double& y_offset)
{
	auto delta_time = ImGui::GetIO().DeltaTime * 100.f;
	x_offset *= m.camera_controls->viewSensitivity * delta_time;
	y_offset *= m.camera_controls->viewSensitivity * delta_time;

	m.camera_controls->yaw -= x_offset;
	m.camera_controls->pitch += y_offset;

	if (m.camera_controls->pitch > 89.f) m.camera_controls->pitch = 89.f;
	if (m.camera_controls->pitch < -89.0f) m.camera_controls->pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m.camera_controls->yaw)) * cos(glm::radians(m.camera_controls->pitch));
	front.y = sin(glm::radians(m.camera_controls->pitch));
	front.z = sin(glm::radians(m.camera_controls->yaw)) * cos(glm::radians(m.camera_controls->pitch));
	m.view_controls->cameraFront = glm::normalize(front);

	update_view();
}

Camera::Camera(WithResultOf&& res) noexcept
{
	auto other = res.func();

	m = std::move(other.m);
}
