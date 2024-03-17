#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <type_traits>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "external/Camera.h"

class Gui {
private:
	struct M {
		GLFWwindow* window;
		std::shared_ptr<Camera::CameraControls> camera_view_controls;
		std::shared_ptr<Camera::ProjectionControls> camera_proj_controls;
	} m;

	void draw_camera_controls_window() const;

	explicit Gui(M m) : m(std::move(m)) {}
public:
	void render_scene(const uint32_t framebuffer_texture, std::unique_ptr<Camera>& camera) const;
	static Gui create(GLFWwindow* window, std::unique_ptr<Camera>& camera);

	void invoke_start() const;


	void invoke_end() const;
	void destroy() const;

	struct WithResultOf {
		std::function<Gui()> func;
	};

	Gui(WithResultOf&& res);
};