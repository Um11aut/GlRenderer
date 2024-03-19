#pragma once

#include <memory>
#include <functional>
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

class Camera {
public:
	struct ProjectionControls {
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;
	};

	struct ViewControls {
		glm::vec3 cameraPosition;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
	};

	struct CameraControls {
		float movementSpeed;
		float viewSensitivity;
		float cameraAcceleration;
		float yaw;
		float pitch;
	};

	static Camera create();

	void update_projection(ImVec2& viewportSize);
	void update_view();
	void process_input(GLFWwindow* window);
	void rotate_camera(double& x_offset, double& y_offset);

	inline std::shared_ptr<glm::mat4> get_proj() const { return m.proj; }
	inline std::shared_ptr<glm::mat4> get_view() const { return m.view; }
	inline std::shared_ptr<ProjectionControls> get_proj_controls() const { return m.proj_controls; }
	inline std::shared_ptr<ViewControls> get_view_controls() const { return m.view_controls; }
	inline std::shared_ptr<CameraControls> get_camera_controls() const { return m.camera_controls; }

	struct WithResultOf {
		std::function<Camera()> func;
	};

	Camera(WithResultOf&& res) noexcept;

private:
	struct M {
		std::shared_ptr<glm::mat4> proj;
		std::shared_ptr<glm::mat4> view;
		std::shared_ptr<ProjectionControls> proj_controls;
		std::shared_ptr<ViewControls> view_controls;
		std::shared_ptr<CameraControls> camera_controls;
	} m;
	
	explicit Camera(M m) : m(std::move(m)) {}
};