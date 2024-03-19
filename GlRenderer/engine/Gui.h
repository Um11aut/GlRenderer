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
		std::shared_ptr<Camera::CameraControls> camera_controls;
		std::shared_ptr<Camera::ProjectionControls> camera_proj_controls;
		std::shared_ptr<ImVec2> scene_viewport_size;
	} m;

	void draw_camera_controls_window() const;

	explicit Gui(M m) : m(std::move(m)) {}
public:
	void render_scene(const uint32_t framebuffer_texture, std::unique_ptr<Camera>& camera) const;
	static Gui create(GLFWwindow* window, std::unique_ptr<Camera>& camera);

	void invoke_start() const;

	void invoke_end() const;
	void destroy() const;

	inline std::shared_ptr<ImVec2> get_scene_viewport_size() const { return m.scene_viewport_size; }

	struct WithResultOf {
		std::function<Gui()> func;
	};

	Gui(WithResultOf&& res);
};